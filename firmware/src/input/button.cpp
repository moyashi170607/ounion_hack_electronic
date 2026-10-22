#include <Arduino.h>

#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

namespace {
// 入力がこの時間だけ変化しなければ、状態を確定する
constexpr unsigned long kDebounceMs = 30;

struct ButtonInput {
    pin_size_t pin;
    Button button;

    int lastRawState = HIGH;      // 前回読み取った生の入力
    int stableState = HIGH;       // チャタリング除去後の状態
    unsigned long changedAt = 0;  // 生の入力が最後に変わった時刻
};

ButtonInput buttons[] = {
    {pins::kSwt1, Button::SOUND0}, {pins::kSwt2, Button::SOUND1},
    {pins::kSwt3, Button::SOUND2}, {pins::kSwt4, Button::SOUND3},
    {pins::kSwt5, Button::SOUND4},
};

}  // namespace

void setup_buttons() {
    for (ButtonInput& input : buttons) {
        pinMode(input.pin, INPUT_PULLUP);

        input.lastRawState = digitalRead(input.pin);
        input.stableState = input.lastRawState;
        input.changedAt = millis();
    }

    hidden_mode =
        digitalRead(pins::kSwt1) == LOW &&
        digitalRead(pins::kSwt2) == LOW &&
        digitalRead(pins::kSwt3) == LOW;
}

void poll_buttons() {
    const unsigned long now = millis();

    for (ButtonInput& input : buttons) {
        const int rawState = digitalRead(input.pin);

        if (rawState != input.lastRawState) {
            input.lastRawState = rawState;
            input.changedAt = now;
        }

        if (now - input.changedAt < kDebounceMs) {
            continue;
        }

        if (rawState == input.stableState) {
            continue;
        }

        input.stableState = rawState;

        if (input.stableState == LOW) {
            pressed_switch(input.button);

            Serial.print("Pressed: SOUND");
            Serial.println(static_cast<unsigned int>(input.button));
        }
    }
}