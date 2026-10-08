#include "display.hpp"

#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "i2s_speaker/i2s_speaker.hpp"
#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

Adafruit_ST7735 tft(&pins::kTftSpi, pins::kTftCs, pins::kTftDc, pins::kTftRst);

/// @brief StepBoxが何行あるか
constexpr size_t kBoxRowsNum = kTracks;

/// @brief StepBoxが何列あるか
constexpr size_t kBoxColsNum = kSteps;

StepBox step_boxs[kBoxRowsNum][kBoxColsNum] = {};

/// @brief StepBoxを全て描写
void init_step_box() {}

/// @brief 現在のBPMを表示
/// @param _bpm
void write_bpm(uint16_t _bpm) {}

void setup_display() {
    pins::kTftSpi.setSCK(pins::kTftSck);
    pins::kTftSpi.setTX(pins::kTftMosi);
    pins::kTftSpi.setRX(NOPIN);

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);

    tft.setCursor(0, 0);

    init_step_box();
}