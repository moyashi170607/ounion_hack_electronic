#include "sequence_manager.hpp"

#include "i2s_speaker/i2s_speaker.hpp"

namespace {

/// @brief 各ステップで鳴らすかどうか（true で頭から再生）
/// @note コア0が書き、コア1が読む
volatile bool pattern[kTracks][kSteps];

}  // namespace

void toggle_step(uint8_t track, uint8_t step) {
    if (track >= kTracks || step >= kSteps) return;
    pattern[track][step] = !pattern[track][step];

    // ボタンが押されたときにこの関数が呼ばれるので、ここで即時再生の処理も入れておく
    // もし変更後がtrue(音がなる状態)だったなら即座に音を鳴らす処理の予約を入れる
    if (pattern[track][step]) {
        trigger_track(track);
    }
}

bool is_step_on(uint8_t track, uint8_t step) {
    if (track >= kTracks || step >= kSteps) return false;
    return pattern[track][step];
}

void pressed_switch(Button button) {
    switch (button) {
        case Button::SOUND0:
            toggle_step(0, current_step());
            break;

        case Button::SOUND1:
            toggle_step(1, current_step());
            break;

        case Button::SOUND2:
            toggle_step(2, current_step());
            break;

        case Button::SOUND3:
            toggle_step(3, current_step());
            break;

        case Button::SOUND4:
            toggle_step(4, current_step());
            break;

        default:
            break;
    }
}