#include <Arduino.h>
#include <LittleFS.h>

#include "i2s_speaker/i2s_speaker.hpp"

// 既定では8KBのスタックを両コアで4KBずつ分け合う。
// dr_wav の drwav_init は1回で約4.5KB使うので、core1 に別途8KBを確保する
bool core1_separate_stack = true;

void setup1() {
    LittleFS.begin();
    load_track(0, "/snare1.wav");
    load_track(1, "/snare2.wav");
    load_track(2, "/hat.wav");
    audio_setup();
}

void loop1() { audio_loop(); }