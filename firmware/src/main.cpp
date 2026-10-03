#include <Arduino.h>

#include "display/display.hpp"
#include "i2s_speaker/i2s_speaker.hpp"

void setup() {
    // LCD周りの初期化
    setup_display();
    tft.setCursor(0, 0);

    pattern[0][0] = pattern[0][4] = true;
    pattern[1][2] = pattern[1][6] = true;

    for (int i = 0; i < 8; i++) {
        pattern[2][i] = true;
    }
}

void loop() {
    // put your main code here, to run repeatedly:
}