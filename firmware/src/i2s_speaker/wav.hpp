#ifndef WAV_HPP
#define WAV_HPP

#include <Arduino.h>
#include <LittleFS.h>

// BackgroundAudioWAV は fmt チャンクサイズ16の PCM ヘッダしか受け付けないため、
// ファイルを解析して44バイトの標準ヘッダを作り直し、ヘッダ → データの順に返す
class WavFile {
   public:
    /// @brief Wavファイルを開く
    /// @param path WavファイルのLittleFS上でのパス
    /// @return 開けたかどうか。失敗時は内部で close する
    bool open(const char* path);

    void close() { file_.close(); }
    bool isOpen() const { return static_cast<bool>(file_); }

    /// @brief 再生位置を先頭に戻す
    /// @details ループ再生用。次の read() はヘッダから返す
    void rewind();

    /// @brief 標準化した44バイトヘッダ → data チャンクの順に、続きを最大
    /// lenバイト返す
    /// @param buf 読み込んだ内容を展開するメモリ
    /// @param len 読み込む最大バイト数
    /// @return buf に書き込んだバイト数。0 なら終端
    size_t read(uint8_t* buf, size_t len);

    /// @brief data チャンクの続きだけを最大 len バイト返す（ヘッダは返さない）
    size_t readData(uint8_t* buf, size_t len);

    uint16_t channels() const { return channels_; }
    uint16_t bitsPerSample() const { return bitsPerSample_; }
    uint32_t sampleRate() const { return sampleRate_; }
    uint32_t dataSize() const { return dataSize_; }

   private:
    bool parse();

    File file_;
    uint8_t header_[44];
    uint32_t dataStart_ = 0;      // ファイル内の音声データ開始位置
    uint32_t dataSize_ = 0;       // 音声データのバイト数
    uint32_t headerSent_ = 0;     // header_ のうち返却済みのバイト数
    uint32_t dataRemaining_ = 0;  // 今回の周回で残っているバイト数

    uint16_t channels_ = 0;
    uint16_t bitsPerSample_ = 0;
    uint32_t sampleRate_ = 0;
};

#endif  // WAV_HPP