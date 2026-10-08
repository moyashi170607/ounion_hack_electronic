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

/// @brief ステップの状態を表す四角形
struct StepBox {
    int16_t x;           // 四角形の左上角のx座標
    int16_t y;           // 四角形の左上角のy座標
    int16_t width;       // 四角形の幅
    int16_t height;      // 四角形の高さ
    uint16_t on_color;   // そのstepの音が鳴っているときの色
    uint16_t off_color;  // そのstepの音が止んでいるときの色
};

#endif  // DISPLAY_HPP
