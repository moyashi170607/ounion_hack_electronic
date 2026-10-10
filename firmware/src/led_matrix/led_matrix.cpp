#include "led_matrix.hpp"

#include "pins.hpp"

bool led_mode[pins::kRowNum][pins::kColNum] = {};

void led_setup()
{
    int i;
    for (i = 0; i < 5; i++)
    {
        pinMode(pins::kLedCols[i], OUTPUT);
    }

    for (i = 5; i < 10; i++)
    {
        pinMode(pins::kLedRows[i], OUTPUT);
    }

    for (i = 9; i > 4; i--)
    {
        digitalwrite(pins::kLedRows[i], HIGH);
    }

    for (i = 4; i > 0; i--)
    {
        digitalwrite(pins::kLedRows[i], LOW);
    }
}

void scan_led() {}