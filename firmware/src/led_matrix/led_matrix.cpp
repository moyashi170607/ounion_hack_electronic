#include "led_matrix.hpp"

#include <Arduino.h>

#include "pins.hpp"

bool led_mode[pins::kRowNum][pins::kColNum] = {};

void led_setup() {
    int i;
    for (i = 0; i < pins::kColNum; i++) {
        pinMode(pins::kLedCols[i], OUTPUT);
    }

    for (i = 0; i < pins::kRowNum; i++) {
        pinMode(pins::kLedRows[i], OUTPUT);
    }

    for (i = 0; i < pins::kRowNum; i++) {
        digitalWrite(pins::kLedRows[i], HIGH);
    }

    for (i = 0; i < pins::kColNum; i++) {
        digitalWrite(pins::kLedCols[i], LOW);
    }
}

pins::LedPos step_led_pos(uint8_t step) {}

pins::LedPos sound_led_pos(uint8_t track) {}

void scan_led() {}