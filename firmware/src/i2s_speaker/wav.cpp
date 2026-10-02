#include "wav.hpp"

namespace {

constexpr uint16_t WAVE_FORMAT_PCM = 0x0001;
constexpr uint16_t WAVE_FORMAT_EXTENSIBLE = 0xFFFE;

uint16_t get16(const uint8_t* p) { return p[0] | (p[1] << 8); }

uint32_t get32(const uint8_t* p) {
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24);
}

void put16(uint8_t* p, uint16_t v) {
    p[0] = v;
    p[1] = v >> 8;
}

void put32(uint8_t* p, uint32_t v) {
    p[0] = v;
    p[1] = v >> 8;
    p[2] = v >> 16;
    p[3] = v >> 24;
}

}  // namespace

bool WavFile::open(const char* path) {
    file_ = LittleFS.open(path, "r");
    if (!file_) return false;
    if (!parse()) {
        close();
        return false;
    }
    rewind();
    return true;
}

void WavFile::rewind() {
    file_.seek(dataStart_);
    headerSent_ = 0;
    dataRemaining_ = dataSize_;
}

size_t WavFile::read(uint8_t* buf, size_t len) {
    size_t n = 0;
    if (headerSent_ < sizeof(header_)) {
        n = min(len, sizeof(header_) - headerSent_);
        memcpy(buf, header_ + headerSent_, n);
        headerSent_ += n;
    }
    return n + readData(buf + n, len - n);
}

size_t WavFile::readData(uint8_t* buf, size_t len) {
    if (len == 0 || dataRemaining_ == 0) return 0;
    const int m = file_.read(buf, min(len, (size_t)dataRemaining_));
    if (m <= 0) return 0;
    dataRemaining_ -= m;
    return m;
}

bool WavFile::parse() {
    uint8_t buf[40];

    // RIFF ヘッダ
    if (file_.read(buf, 12) != 12) return false;
    if (memcmp(buf, "RIFF", 4) != 0 || memcmp(buf + 8, "WAVE", 4) != 0) {
        return false;
    }

    const uint32_t fileSize = file_.size();

    bool hasFmt = false;
    bool hasData = false;
    uint16_t channels = 0;
    uint16_t bitsPerSample = 0;
    uint32_t sampleRate = 0;

    // fmt と data が揃うまでチャンクを順に読む (LIST などは読み飛ばす)
    while (!(hasFmt && hasData)) {
        const uint32_t body = file_.position() + 8;

        // チャンクヘッダ(8バイト)を読む余地がなければ打ち切る
        if (body > fileSize) break;
        if (file_.read(buf, 8) != 8) return false;

        const uint32_t size = get32(buf + 4);

        // fmtなら
        if (memcmp(buf, "fmt ", 4) == 0) {
            // PCM の fmt は最低16バイト必要
            if (size < 16) return false;

            // 読むバイトを決める
            const size_t n = min(size, (uint32_t)sizeof(buf));
            // 決めた分読めなかったら解析失敗
            if (file_.read(buf, n) != (int)n) return false;

            uint16_t format = get16(buf);
            // EXTENSIBLE の場合は SubFormat GUID の先頭2バイトが実際の形式
            if (format == WAVE_FORMAT_EXTENSIBLE && size >= 40) {
                format = get16(buf + 24);
            }

            // WAVの形式にあっていなければ解析失敗
            if (format != WAVE_FORMAT_PCM) return false;

            // 情報を取得
            channels = get16(buf + 2);
            sampleRate = get32(buf + 4);
            bitsPerSample = get16(buf + 14);
            hasFmt = true;
        }
        // dataなら
        else if (memcmp(buf, "data", 4) == 0) {
            dataStart_ = body;
            dataSize_ = min(size, fileSize - body);
            hasData = true;
        }

        // チャンクは2バイト境界に揃えられている
        const uint64_t next = (uint64_t)body + size + (size & 1);
        if (next > fileSize) break;
        file_.seek(next);
    }

    // そろわずに中断したなら解析失敗
    if (!hasFmt || !hasData) return false;

    // BackgroundAudioWAV が再生できる範囲に限る
    if (channels < 1 || channels > 2) return false;
    if (bitsPerSample != 8 && bitsPerSample != 16) return false;
    if (sampleRate < 4000 || sampleRate > 48000) return false;

    // デコーダはサンプル単位で消費するため、端数が残ると
    // ループ時に次のヘッダとずれる
    const uint16_t blockAlign = channels * bitsPerSample / 8;
    dataSize_ -= dataSize_ % blockAlign;
    if (dataSize_ == 0) return false;

    // 形式を変換したものを組み立てる
    memcpy(header_, "RIFF", 4);
    put32(header_ + 4, 36 + dataSize_);
    memcpy(header_ + 8, "WAVE", 4);
    memcpy(header_ + 12, "fmt ", 4);
    put32(header_ + 16, 16);
    put16(header_ + 20, WAVE_FORMAT_PCM);
    put16(header_ + 22, channels);
    put32(header_ + 24, sampleRate);
    put32(header_ + 28, sampleRate * blockAlign);
    put16(header_ + 32, blockAlign);
    put16(header_ + 34, bitsPerSample);
    memcpy(header_ + 36, "data", 4);
    put32(header_ + 40, dataSize_);

    channels_ = channels;
    bitsPerSample_ = bitsPerSample;
    sampleRate_ = sampleRate;

    return true;
}