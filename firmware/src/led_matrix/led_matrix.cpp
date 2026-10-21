#include "led_matrix.hpp"

#include <Arduino.h>

#include "i2s_speaker/i2s_speaker.hpp"
#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

volatile bool led_mode[pins::kRowNum][pins::kColNum] = {};

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

void clear_led_mode() {
    int i, j;
    for (i = 0; i < pins::kRowNum; i++) {
        for (j = 0; j < pins::kColNum; j++) {
            led_mode[i][j] = false;
        }
    }
}

void update_step_led() {
    int i;

    for (i = 0; i < kSteps; i++) {
        const pins::LedPos& pos = pins::kStepLeds[i];

        if (i == current_step()) {
            led_mode[pos.row][pos.col] = true;
        } else {
            led_mode[pos.row][pos.col] = false;
        }
    }
}

void update_track_led() {
    int i, step;
    step = current_step();
    for (i = 0; i < kTracks; i++) {
        const pins::LedPos& trackspos = pins::kSoundLeds[i];

        if (is_step_on(i, step) == true) {
            led_mode[trackspos.row][trackspos.col] = true;
        } else {
            led_mode[trackspos.row][trackspos.col] = false;
        }
    }
}

void scan_led() {
    // 現在走査中の行
    static size_t row = 0;

    // 一度全部光らない状態にする
    digitalWrite(pins::kLedRows[row], HIGH);

    // row + 1をpins::kRowNumで割ったあまりが代入される
    row = (row + 1) % pins::kRowNum;

    // led_modeに対応した列を光る状態にする
    for (size_t col = 0; col < pins::kColNum; col++) {
        digitalWrite(pins::kLedCols[col], led_mode[row][col] ? HIGH : LOW);
    }

    // 光らせる
    digitalWrite(pins::kLedRows[row], LOW);
}
