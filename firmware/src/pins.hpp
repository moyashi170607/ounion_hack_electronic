#ifndef PINS_HPP
#define PINS_HPP

#include <SPI.h>

#include <iterator>

#include "sequence_manager/sequence_manager.hpp"

namespace pins {

// LCD SPI
// TODO: GPIOの割り当ては再度検討する必要あり
// 参照は const 変数と違って内部リンケージにならないので inline が必要
inline constexpr auto& kTftSpi = SPI1;
constexpr pin_size_t kTftSck = 10;   // SCK
constexpr pin_size_t kTftMosi = 11;  // SDA
constexpr pin_size_t kTftCs = 13;    // CS
constexpr pin_size_t kTftDc = 12;    // A0
constexpr pin_size_t kTftRst = 15;   // RESET

// I2S DAC
constexpr pin_size_t kDacBck = 20;  // BCK
constexpr pin_size_t kDacLck =
    kDacBck + 1;                    // LCK (BCK+1 に自動で割り当てられる)
constexpr pin_size_t kDacDin = 22;  // DIN

// スイッチのピン
constexpr pin_size_t kSwt1 = 16;
constexpr pin_size_t kSwt2 = 17;
constexpr pin_size_t kSwt3 = 18;
constexpr pin_size_t kSwt4 = 19;
constexpr pin_size_t kSwt5 = 26;

// マトリックスLEDのピン
constexpr pin_size_t kLedCols[] = {0, 1, 2, 3, 4};
constexpr pin_size_t kLedRows[] = {5, 6, 7, 8, 9};

/// @brief マトリックスの行数・列数
constexpr size_t kRowNum = std::size(pins::kLedRows);
constexpr size_t kColNum = std::size(pins::kLedCols);

/// @brief マトリックス上のLEDの位置
struct LedPos {
    uint8_t row;
    uint8_t col;
};

/// @brief ステップ番号 → LEDの位置（配線に合わせて書き換える）
constexpr LedPos kStepLeds[] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},  // ステップ 0〜4
    {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4},  // ステップ 5〜9
    {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4},  // ステップ 10〜14
    {3, 0},                                  // ステップ 15
};

constexpr LedPos kSoundLeds[] = {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {4, 0}};

static_assert(std::size(kStepLeds) == kSteps,
              "kStepLeds の数が kSteps と合わない");
static_assert(std::size(kSoundLeds) == kTracks,
              "kSoundLeds の数が kTracks と合わない");

}  // namespace pins

#endif  // PINS_HPP
