#include "display.hpp"

#include <SPI.h>

#include "pins.hpp"

Adafruit_ST7735 tft(&TFT_SPI, TFT_CS, TFT_DC, TFT_RST);

void setup_display() {
    TFT_SPI.setSCK(TFT_SCK);
    TFT_SPI.setTX(TFT_MOSI);
    TFT_SPI.setRX(NOPIN);

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
}