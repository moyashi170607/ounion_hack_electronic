#include "led_matrix.hpp"

#include "pins.hpp"

bool led_mode[pins::kRowNum][pins::kColNum] = {};

void led_setup()
{
    int i;
    for (i = 0; i < kRowNum; i++)
    {
        pinMode(pins::kLedCols[i], OUTPUT);
    }

    for (i = 0; i < kColNum; i++)
    {
        pinMode(pins::kLedRows[i], OUTPUT);
    }

    for (i = 0; i > kRowNum; i++)
    {
        digitalwrite(pins::kLedRows[i], HIGH);
    }

    for (i = 0; i > kColNum; i++)
    {
        digitalwrite(pins::kLedRows[i], LOW);
    }
}

void scan_led() {}