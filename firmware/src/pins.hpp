#ifndef PINS_HPP
#define PINS_HPP

#include <SPI.h>

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
constexpr pin_size_t kSwt1 = 21;
constexpr pin_size_t kSwt2 = 22;
constexpr pin_size_t kSwt3 = 24;
constexpr pin_size_t kSwt4 = 25;
constexpr pin_size_t kSwt5 = 31;

}  // namespace pins

#endif  // PINS_HPP
