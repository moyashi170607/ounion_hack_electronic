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

/// @brief ボタンが押されたことを受け取る
/// @param button
void pressed_switch(Button button);

#endif  // SEQUENCE_MANAGER_HPP