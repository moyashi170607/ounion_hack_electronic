#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Adafruit_ST7735.h>
#include <SPI.h>

extern Adafruit_ST7735 tft;

/// @brief LCDを初期化する関数
/// @param tft Adafruit_ST7735の参照
/// @param spi SPIClassRP2040の参照
/// @param sck TFTするGPIO
/// @param mosi TFTするGPIO
void setup_display();
void display_print();

void drawBlock(int step, int instrument, uint16_t color);
void drawAllBlocks();

#endif  // DISPLAY_HPP
