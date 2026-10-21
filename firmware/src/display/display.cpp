#include "display.hpp"

#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

Adafruit_ST7735 tft(&pins::kTftSpi, pins::kTftCs, pins::kTftDc, pins::kTftRst);

void setup_display() {
    pins::kTftSpi.setSCK(pins::kTftSck);  // clock
    pins::kTftSpi.setTX(pins::kTftMosi);  // transmit
    pins::kTftSpi.setRX(NOPIN);           // receive

    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
}

void display_print() {
    tft.drawPixel(10, 10, ST77XX_WHITE);        // x, y color. 1 pixel
    tft.drawLine(10, 20, 100, 20, ST77XX_RED);  // x0, y0, x1, y1, color. 1 line
    tft.drawRect(10, 30, 50, 30,
                 ST77XX_GREEN);  // x, y, w, h, color. 1 rectangle
    tft.fillRect(70, 30, 40, 30,
                 ST77XX_BLUE);  // x, y, w, h, color. 1 filled rectangle
    tft.drawCircle(30, 90, 15, ST77XX_YELLOW);  // x, y, r, color. 1 circle

    // 文字
    tft.setCursor(60, 80);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("Hello");
}

struct Position {
    int x;
    int y;
};

constexpr int BLOCK_W = 10;
constexpr int BLOCK_H = 10;
constexpr int NUM_TRACKS = kTracks;
constexpr int NUM_STEPS = kSteps;

constexpr int STEP_COLOR = ST77XX_GREEN;
static bool prev_state[NUM_TRACKS][NUM_STEPS] = {};

Position get_block_pos(int step, int track) {
    constexpr int X_o = 2 + 62;
    constexpr int Y_o = 2;
    constexpr int ROW_LENGTH = 8;
    constexpr int INTERVAL_X = BLOCK_W + 2;
    constexpr int INTERVAL_Y = BLOCK_H + 2;
    constexpr int TRACK_SPACE_Y = INTERVAL_Y * 2 + 2;

    int col = step % ROW_LENGTH;     // 0〜7
    int subRow = step / ROW_LENGTH;  // 0 or 1

    int x = X_o + col * INTERVAL_X;
    int y = Y_o + subRow * INTERVAL_Y + track * TRACK_SPACE_Y;

    return {x, y};
}

void draw_block(int step, int track, uint16_t color) {
    Position p = get_block_pos(step, track);

    tft.fillRect(p.x, p.y, BLOCK_W, BLOCK_H, color);
    tft.drawRect(p.x, p.y, BLOCK_W, BLOCK_H, ST77XX_WHITE);
}

void draw_all_blocks() {
    for (int i = 0; i < NUM_TRACKS; i++) {
        for (int j = 0; j < NUM_STEPS; j++) {
            draw_block(j, i, ST77XX_BLACK);
        }
    }
}

// トラックの状態に合わせて四角形の色が変わる
// is_step_on(uint8_t track, uint8_t step);で指定したtrack（楽器）の指定したstepがONかOFFか取得
void update_block() {
    for (int track = 0; track < NUM_TRACKS; track++) {
        for (int step = 0; step < NUM_STEPS; step++) {

            bool current_state = is_step_on(track, step); // 各ブロックのステップがON/OFFかを取得

            if (current_state == prev_state[track][step]) {
                continue;
            }

            // 状態が変化した場合のみ描画を更新
            prev_state[track][step] = current_state;

            uint16_t color = current_state? STEP_COLOR : ST77XX_BLACK;
            draw_block(step, track, color); // 枠線を描画
        }
    }
}