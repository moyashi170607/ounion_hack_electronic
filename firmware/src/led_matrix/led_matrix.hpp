#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

#include <Arduino.h>

#include "i2s_speaker/i2s_speaker.hpp"
#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

/// @brief LEDのピンを全てOUTPUTモードにしてLEDが消えている状態にする
void led_setup();

/// @brief led_modeを全てfalseにする
void clear_led_mode();

/// @brief stepに関するLEDのled_modeを変更する
void update_step_led();

/// @brief LEDを点灯消灯させる
/// @note set_xxx_ledはあくまでもどこを点灯させるかの設定をするのみで、
/// 実際にピンのHIGH LOWを切り替えるのはこの関数
void scan_led();

#endif  // !LED_MATRIX_HPP
