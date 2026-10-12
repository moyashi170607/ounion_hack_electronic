#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

#include <Arduino.h>

#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

/// @brief LEDのピンを全てOUTPUTモードにしてLEDが消えている状態にする
void led_setup();

/// @brief LEDを点灯消灯させる
/// @note set_xxx_ledはあくまでもどこを点灯させるかの設定をするのみで、
/// 実際にピンのHIGH LOWを切り替えるのはこの関数
void scan_led();

#endif  // !LED_MATRIX_HPP
