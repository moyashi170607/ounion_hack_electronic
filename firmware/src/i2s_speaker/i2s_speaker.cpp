#include "i2s_speaker.hpp"

#include <I2S.h>

#include "pins.hpp"
#include "wav.hpp"

Track tracks[kTracks];
volatile bool pattern[kTracks][kSteps];
volatile uint16_t bpm = 120;

namespace {

I2S i2s(OUTPUT);

/// @brief pos フレーム目を16bitの L/R にそろえて取り出す
void read_frame(const Track& tr, uint32_t pos, int32_t& l, int32_t& r) {
    const uint32_t i = pos * tr.channels;
    if (tr.bits == 8) {
        // 8bit WAV は符号なし（無音 = 128）なので中心を0にずらして256倍
        const uint8_t* p = static_cast<const uint8_t*>(tr.data);
        l = (p[i] - 128) * 256;
        r = (tr.channels == 2) ? (p[i + 1] - 128) * 256 : l;
    } else {
        const int16_t* p = static_cast<const int16_t*>(tr.data);
        l = p[i];
        r = (tr.channels == 2) ? p[i + 1] : l;
    }
}

}  // namespace

bool load_track(size_t t, const char* path) {
    if (t >= kTracks) return false;

    WavFile wav;
    if (!wav.open(path)) return false;

    // ミキサーは kSampleRate 固定で回すので、違うレートだと再生速度が狂う
    if (wav.sampleRate() != kSampleRate) {
        wav.close();
        return false;
    }

    // malloc は8バイト境界なので int16_t として読んでも安全
    const uint32_t size = wav.dataSize();
    uint8_t* buf = static_cast<uint8_t*>(malloc(size));
    if (!buf) {
        wav.close();
        return false;
    }
    const size_t n = wav.readData(buf, size);
    wav.close();
    if (n != size) {
        free(buf);
        return false;
    }

    Track& tr = tracks[t];
    tr.data = buf;
    tr.channels = wav.channels();
    tr.bits = wav.bitsPerSample();
    tr.frames = size / (tr.channels * tr.bits / 8);
    tr.pos = tr.frames;  // 停止状態から始める
    return true;
}

void audio_setup() {
    i2s.setBCLK(pins::kDacBck);  // LRCLK は BCLK+1
    i2s.setDATA(pins::kDacDin);
    i2s.setBitsPerSample(16);
    i2s.setBuffers(4, 64);  // 小さいほど遅延が短い
    i2s.begin(kSampleRate);
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
    for (auto& tr : tracks) {
        if (tr.pos >= tr.frames) continue;
        int32_t l, r;
        read_frame(tr, tr.pos++, l, r);
        mixL += (l * tr.gain) >> 8;
        mixR += (r * tr.gain) >> 8;
    }
    mixL = constrain(mixL, -32767, 32767);
    mixR = constrain(mixR, -32767, 32767);
    i2s.write16(mixL, mixR);  // バッファが空くまで待つ
}
