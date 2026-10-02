#include <Arduino.h>
#include <LittleFS.h>

#include "i2s_speaker/i2s_speaker.hpp"

void setup1() {
    LittleFS.begin();
    load_track(0, "/kick.wav");
    load_track(1, "/snare.wav");
    audio_setup();
}

void loop1() { audio_loop(); }