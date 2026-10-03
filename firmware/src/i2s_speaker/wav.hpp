#ifndef WAV_HPP
#define WAV_HPP

#include <Arduino.h>
#include <LittleFS.h>
#include <dr_wav.h>

class WavFile {
   public:
    WavFile() = default;
    ~WavFile() { close(); }

    // wav_ が &file_ を握っているのでコピー禁止
    WavFile(const WavFile&) = delete;
    WavFile& operator=(const WavFile&) = delete;

    /// @brief Wavファイルを開く
    /// @param path WavファイルのLittleFS上でのパス
    /// @return 開けたかどうか。失敗時は内部で close する
    bool open(const char* path);

    void close();
    bool isOpen() const { return opened_; }

    /// @brief 再生位置を先頭フレームに戻す
    bool rewind() { return drwav_seek_to_pcm_frame(&wav_, 0); }
    /// @brief 16bit符号付きに変換して最大 frames フレーム読む
    /// @param buf frames * channels() 個の int16_t が入る領域（ステレオは L, R
    /// の交互）
    /// @return 読めたフレーム数。0 なら終端
    uint64_t readFramesS16(int16_t* buf, uint64_t frames) {
        return drwav_read_pcm_frames_s16(&wav_, frames, buf);
    }

    uint16_t channels() const { return wav_.channels; }
    uint32_t sampleRate() const { return wav_.sampleRate; }
    uint64_t totalFrames() const { return wav_.totalPCMFrameCount; }

   private:
    File file_;
    drwav wav_{};
    bool opened_ = false;

    // dr_wav から呼ばれる File への読み書き
    static size_t onRead(void* user, void* buf, size_t len);
    // 読む位置の変更
    // origin: DRWAV_SEEK_SET=先頭から / DRWAV_SEEK_CUR=現在位置から /
    // DRWAV_SEEK_END=末尾から
    static drwav_bool32 onSeek(void* user, int offset,
                               drwav_seek_origin origin);
    // 現在の読み込み位置を取得
    static drwav_bool32 onTell(void* user, drwav_int64* cursor);
};

#endif  // WAV_HPP