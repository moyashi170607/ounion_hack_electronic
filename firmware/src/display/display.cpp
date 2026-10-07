#include "display.hpp"

#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "pins.hpp"

Adafruit_ST7735 tft(&pins::kTftSpi, pins::kTftCs, pins::kTftDc, pins::kTftRst);

void setup_display() {
    pins::kTftSpi.setSCK(pins::kTftSck); // clock
    pins::kTftSpi.setTX(pins::kTftMosi); // transmit
    pins::kTftSpi.setRX(NOPIN); // receive

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
}

void display_print() {
    tft.drawPixel(10, 10, ST77XX_WHITE);        // x, y color. 1 pixel
    tft.drawLine(10, 20, 100, 20, ST77XX_RED);  // x0, y0, x1, y1, color. 1 line
    tft.drawRect(10, 30, 50, 30, ST77XX_GREEN); // x, y, w, h, color. 1 rectangle
    tft.fillRect(70, 30, 40, 30, ST77XX_BLUE);  // x, y, w, h, color. 1 filled rectangle
    tft.drawCircle(30, 90, 15, ST77XX_YELLOW);  // x, y, r, color. 1 circle

    // 文字
    tft.setCursor(60, 80);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.print("Hello");
}

void display_squares() {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 16; x++) {
            int px = x * 10;
            int py = y * 20;

            tft.drawRect(px, py, 8, 18, ST77XX_WHITE);
        }
    }
}
