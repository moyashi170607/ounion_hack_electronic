#include <Arduino.h>

#include "display/display.hpp"
#include "i2s_speaker/i2s_speaker.hpp"
#include "led_matrix/led_matrix.hpp"
#include "sequence_manager/sequence_manager.hpp"

#include "pins.hpp"

void setup() {
    // LCD周りの初期化
    setup_display();
    tft.setCursor(0, 0);

    Serial.begin(115200);
    pinMode(pins:: kSwt1, INPUT_PULLUUP);
}


void loop() {
    // put your main code here, to run repeatedly:
    Serial.println()
}