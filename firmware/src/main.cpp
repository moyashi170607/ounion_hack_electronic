#include <Arduino.h>

#include "display/display.hpp"
#include "i2s_speaker/i2s_speaker.hpp"

void setup() {
    // LCD周りの初期化
    setup_display();
    // tft.setCursor(0, 0);
    display_print();
}

void loop() {
    // put your main code here, to run repeatedly:
    
}