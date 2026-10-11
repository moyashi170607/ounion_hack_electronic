#include <Arduino.h>

#include "display/display.hpp"
#include "i2s_speaker/i2s_speaker.hpp"
#include "led_matrix/led_matrix.hpp"
#include "sequence_manager/sequence_manager.hpp"
#include "pins.hpp"

void setup_buttons() {
    for (ButtonInput& input : buttons) {
        pinMode(input.pin, INPUT_PULLUP);

        input.lastRawState = digitalRead(input.pin);
        input.stableState = input.lastRawState;
        input.changedAt = millis();
    }
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

void setup() {
    setup_display();
    tft.setCursor(0, 0);

    Serial.begin(115200);
    setup_buttons();  // ボタンの初期化
}

void loop() {
    poll_buttons();  // 毎回ボタンを確認

    // 今後、ほかの処理もここに追加できる
}