#include <Arduino.h>

#include "display/display.hpp"
#include "i2s_speaker/i2s_speaker.hpp"
#include "input/button.hpp"
#include "led_matrix/led_matrix.hpp"
#include "pins.hpp"
#include "sequence_manager/sequence_manager.hpp"

void setup() {
    setup_buttons();
    
    setup_display();
    draw_all_blocks();
  
    Serial.begin(115200);
}


void loop() {
    poll_buttons();
}
