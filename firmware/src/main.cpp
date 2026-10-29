#include <Arduino.h>
#include <pico/time.h>

#include "display/display.hpp"
#include "i2s_speaker/i2s_speaker.hpp"
#include "input/button.hpp"
#include "led_matrix/led_matrix.hpp"
#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

namespace {
repeating_timer_t led_timer;
}

bool on_led_timer(repeating_timer* t) {
    scan_led();
    return true;
}

void setup() {
    setup_buttons();

    // もう一つのコアにモードを伝える
    rp2040.fifo.push(hidden_mode);

    setup_display();
    draw_all_blocks();

    Serial.begin(115200);

    led_setup();
    add_repeating_timer_us(-1000, on_led_timer, nullptr, &led_timer);
}

void loop() {
    poll_buttons();
    update_step_led();
    update_track_led();
    update_block();
}
