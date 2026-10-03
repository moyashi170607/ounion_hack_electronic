#include "i2s_speaker.hpp"

#include <I2S.h>

#include "pins.hpp"
#include "wav.hpp"

Track tracks[kTracks];
volatile bool pattern[kTracks][kSteps];
volatile uint16_t bpm = 120;

namespace {

I2S i2s(OUTPUT);

/// @brief I2S へは kSampleRate の何倍で出すか
/// @note PCM5102A は 22.05kHz を正式にサポートしておらず無音になることがあるので、
///       各サンプルを2回ずつ送って 44.1kHz で出す
constexpr uint32_t kOversample = 2;

/// @brief pos フレーム目を16bitの L/R にそろえて取り出す
void read_frame(const Track& tr, uint32_t pos, int32_t& l, int32_t& r) {
    const int16_t* p = tr.data + pos * tr.channels;
    l = p[0];
    r = (tr.channels == 2) ? p[1] : l;
}

}  // namespace

bool load_track(size_t t, const char* path) {
    if (t >= kTracks) return false;

    WavFile wav;
    if (!wav.open(path)) return false;

    // ミキサーは kSampleRate 固定で回すので、違うレートだと再生速度が狂う
    if (wav.sampleRate() != kSampleRate) return false;

    const uint16_t channels = wav.channels();

    // チャンネル数が1か2でなければ弾く
    if (channels < 1 || channels > 2) return false;

    const uint64_t frames = wav.totalFrames();
    // フレームが0なら弾く
    if (frames == 0) return false;

    int16_t* buf =
        static_cast<int16_t*>(malloc(frames * channels * sizeof(int16_t)));
    if (!buf) return false;

    // dr_wavで16bit符号付きにそろえる
    const uint64_t n = wav.readFramesS16(buf, frames);
    if (n == 0) {
        free(buf);
        return false;
    }

    Track& tr = tracks[t];
    tr.data = buf;
    tr.channels = channels;
    tr.frames = n;
    tr.pos = n;  // 停止状態から始める
    return true;
}

void audio_setup() {
    i2s.setBCLK(pins::kDacBck);  // LRCLK は BCLK+1
    i2s.setDATA(pins::kDacDin);
    i2s.setBitsPerSample(16);
    i2s.setBuffers(4, 64);  // 小さいほど遅延が短い
    i2s.begin(kSampleRate * kOversample);
}

void audio_loop() {
    // 1ステップ = kSampleRate*60/(bpm*4) サンプル。
    // 割り切れない端数も持ち越すのでテンポがずれない
    static uint32_t phase = kSampleRate * 60;
    static uint32_t step = kSteps - 1;
    phase += bpm * 4;
    if (phase >= kSampleRate * 60) {
        phase -= kSampleRate * 60;
        step = (step + 1) % kSteps;
        for (size_t t = 0; t < kTracks; t++)
            if (pattern[t][step]) tracks[t].pos = 0;  // 頭から鳴らし直す
    }

    int32_t mixL = 0;
    int32_t mixR = 0;
    for (Track& tr : tracks) {
        if (tr.pos >= tr.frames) continue;
        int32_t l, r;
        read_frame(tr, tr.pos++, l, r);

        // 256で割ることで割合に戻している
        mixL += (l * tr.gain) >> 8;
        mixR += (r * tr.gain) >> 8;
    }
    mixL = constrain(mixL, -32767, 32767);
    mixR = constrain(mixR, -32767, 32767);
    // バッファが空くまで待つ
    for (uint32_t i = 0; i < kOversample; i++) i2s.write16(mixL, mixR);
}
