#ifndef I2S_SPEAKER_HPP
#define I2S_SPEAKER_HPP

#include <Arduino.h>

/// @brief 1トラック分の音データと再生状態
struct Track {
    const int16_t* data =
        nullptr;           // 16bit符号付きPCM（ステレオは L, R の交互）
    uint32_t frames = 0;   // フレーム数（ステレオは L/R の組で1）
    uint32_t pos = 0;      // 再生位置[フレーム]。frames 以上なら停止中
    uint8_t channels = 1;  // 1: モノラル, 2: ステレオ（L, R の交互）
    int32_t gain = 256;    // 256 = 1.0
};

/// @brief サンプリング周波数 [Hz]
constexpr uint32_t kSampleRate = 22050;

/// @brief トラック数
constexpr size_t kTracks = 5;

/// @brief ステップ数
constexpr size_t kSteps = 16;

/// @brief 各トラックの音データと再生位置
/// @note pos は loop1（コア1）が進める
extern Track tracks[kTracks];

/// @brief 各ステップで鳴らすかどうか（true で頭から再生）
/// @note コア0が書き、コア1が読む
extern volatile bool pattern[kTracks][kSteps];

/// @brief テンポ [BPM]。1ステップは16分音符
extern volatile uint16_t bpm;

/// @brief WAVファイルをRAMに読み込んで tracks[t] に割り当てる関数
/// @note audio_setup() より前に呼ぶ
bool load_track(size_t t, const char* path);

/// @brief I2Sを初期化する関数
void audio_setup();

/// @brief 1サンプル分ミックスしてI2Sへ出力する関数
/// @note バッファが空くまでブロックするので、loop1（コア1）から毎回呼ぶ
void audio_loop();

#endif  // I2S_SPEAKER_HPP
