#include "led_matrix.hpp"

#include "pins.hpp"

bool led_mode[pins::kRowNum][pins::kColNum] = {};

void led_setup()
{
    pinMode(pins::kLedRows[5], OUTPUT);
    pinMode(pins::kLedRows[6], OUTPUT);
    pinMode(pins::kLedRows[7], OUTPUT);
    pinMode(pins::kLedRows[8], OUTPUT);
    pinMode(pins::kLedRows[9], output);

    pinMode(pins::kLedCols[0], OUTPUT);
    pinMode(pins::kLedCols[1], OUTPUT);
    pinMode(pins::kLedCols[2], OUTPUT);
    pinMode(pins::kLedCols[3], OUTPUT);
    pinMode(pins::kLedCols[4], OUTPUT);

    digitalwrite(pins::kLedRows[5], HIGH);
    digitalwrite(pins::kLedRows[6], HIGH);
    digitalwrite(pins::kLedRows[7], HIGH);
    digitalwrite(pins::kLedRows[8], HIGH);
    digitalwrite(pins::kLedRows[9], HIGH);

    digitalwrite(pins::kLedCols[0], LOW);
    digitalwrite(pins::kLedCols[1], LOW);
    digitalwrite(pins::kLedCols[2], LOW);
    digitalwrite(pins::kLedCols[3], LOW);
    digitalwrite(pins::kLedCols[4], LOW);
}

void scan_led() {}