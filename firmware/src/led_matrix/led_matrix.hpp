#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

#include <Arduino.h>

#include "sequence_manager/sequence_manager.hpp"

/// @brief LEDのピンを全てOUTPUTモードにしてLEDが消えている状態にする
void led_setup();

/// @brief
/// 前のステップのLEDを消し、今のステップを表すLEDを点灯するように設定する
/// @param step 光らせるステップ
void set_step_led(uint8_t step);

/// @brief ボタンのそばにあるLEDを制御する関数
/// @param track どのトラックに対応したLEDか
/// @param on trueなら点灯、falseなら消灯
void set_sound_led(uint8_t track, bool on);

/// @brief LEDを点灯消灯させる
/// @note set_xxx_ledはあくまでもどこを点灯させるかの設定をするのみで、
/// 実際にピンのHIGH LOWを切り替えるのはこの関数
void scan_led();

#endif  // !LED_MATRIX_HPP
