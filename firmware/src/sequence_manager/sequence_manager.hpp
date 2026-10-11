#ifndef SEQUENCE_MANAGER_HPP
#define SEQUENCE_MANAGER_HPP

#include <Arduino.h>

/// @brief トラック数
constexpr size_t kTracks = 5;

/// @brief ステップ数
constexpr size_t kSteps = 16;

enum class Button : uint8_t {
    SOUND0,
    SOUND1,
    SOUND2,
    SOUND3,
    SOUND4,
};

/// @brief 指定したトラック・ステップの ON/OFF を反転する
void toggle_step(uint8_t track, uint8_t step);

/// @brief 指定したトラック・ステップが ON かどうか
/// @note コア1（audio_loop）から呼ばれる
bool is_step_on(uint8_t track, uint8_t step);

/// @brief ボタンが押されたことを受け取る
/// @param button
void pressed_switch(Button button);

#endif  // SEQUENCE_MANAGER_HPP