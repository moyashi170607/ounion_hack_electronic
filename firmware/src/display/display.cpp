#include "display.hpp"

#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "i2s_speaker/i2s_speaker.hpp"
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
constexpr uint16_t FRAME_COLOR = ST77XX_WHITE;
constexpr uint16_t CURRENT_FRAME_COLOR = ST77XX_RED;  // 現在のステップの枠の色
static bool prev_state[NUM_TRACKS][NUM_STEPS] = {};
static int prev_step = -1;  // 枠を強調して描画済みのステップ（-1 はまだ無し）

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

uint16_t get_frame_color(int step) {
    return step == prev_step ? CURRENT_FRAME_COLOR : FRAME_COLOR;
}

void draw_block(int step, int track, uint16_t color) {
    Position p = get_block_pos(step, track);

    tft.fillRect(p.x, p.y, BLOCK_W, BLOCK_H, color);
    tft.drawRect(p.x, p.y, BLOCK_W, BLOCK_H, get_frame_color(step));
}

// 中身は塗り直さず枠線だけ描き直す
void draw_frame(int step, int track) {
    Position p = get_block_pos(step, track);

    tft.drawRect(p.x, p.y, BLOCK_W, BLOCK_H, get_frame_color(step));
}

// 全部オフの状態でのブロックの描画
void draw_all_blocks() {
    for (int track = 0; track < NUM_TRACKS; track++) {
        for (int step = 0; step < NUM_STEPS; step++) {
            draw_block(step, track, ST77XX_BLACK);
            prev_state[track][step] = false;
        }
    }
}

// 実際のステップの状態と同期させ，差分となるブロックのみ色を変える
void update_block() {
    // ステップが進んだら，前のステップの枠を戻して今のステップの枠を強調する
    int step_now = current_step();
    if (step_now != prev_step) {
        int old_step = prev_step;
        prev_step = step_now;

        for (int track = 0; track < NUM_TRACKS; track++) {
            if (old_step >= 0) {
                draw_frame(old_step, track);
            }
            draw_frame(step_now, track);
        }
    }

    for (int track = 0; track < NUM_TRACKS; track++) {
        for (int step = 0; step < NUM_STEPS; step++) {
            bool current_state = is_step_on(
                track, step);  // 各ブロックのステップがON/OFFかを取得

            if (current_state == prev_state[track][step]) {
                continue;
            }

            // 状態が変化した場合のみ描画を更新
            prev_state[track][step] = current_state;

            uint16_t color = current_state ? STEP_COLOR : ST77XX_BLACK;
            draw_block(step, track, color);  // 枠線を描画
        }
    }
}