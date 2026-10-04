#include "display.hpp"

#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "pins.hpp"

Adafruit_ST7735 tft(&pins::kTftSpi, pins::kTftCs, pins::kTftDc, pins::kTftRst);

void setup_display() {
    pins::kTftSpi.setSCK(pins::kTftSck);
    pins::kTftSpi.setTX(pins::kTftMosi);
    pins::kTftSpi.setRX(NOPIN);

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
}