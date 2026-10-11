#include "sequence_manager.hpp"

namespace {

/// @brief 各ステップで鳴らすかどうか（true で頭から再生）
/// @note コア0が書き、コア1が読む
volatile bool pattern[kTracks][kSteps];

}  // namespace

void toggle_step(uint8_t track, uint8_t step) {
    if (track >= kTracks || step >= kSteps) return;
    pattern[track][step] = !pattern[track][step];
}

bool is_step_on(uint8_t track, uint8_t step) {
    if (track >= kTracks || step >= kSteps) return false;
    return pattern[track][step];
}

void pressed_switch(Button button) {}