#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

#include <Arduino.h>

#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

/// @brief LEDのピンを全てOUTPUTモードにしてLEDが消えている状態にする
void led_setup();

/// @brief ステップ番号に対応するLEDの位置を返す
/// @param step ステップ番号（0 〜 kSteps-1）
/// @return LEDの位置。範囲外なら kNoLedPos
pins::LedPos step_led_pos(uint8_t step);

/// @brief トラック番号に対応するLEDの位置を返す
/// @param track トラック番号（0 〜 kTracks-1）
/// @return LEDの位置。範囲外なら kNoLedPos
pins::LedPos sound_led_pos(uint8_t track);

/// @brief LEDを点灯消灯させる
/// @note set_xxx_ledはあくまでもどこを点灯させるかの設定をするのみで、
/// 実際にピンのHIGH LOWを切り替えるのはこの関数
void scan_led();

#endif  // !LED_MATRIX_HPP
