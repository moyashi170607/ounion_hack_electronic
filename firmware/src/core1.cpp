#include <Arduino.h>
#include <LittleFS.h>

#include "i2s_speaker/i2s_speaker.hpp"

// 既定では8KBのスタックを両コアで4KBずつ分け合う。
// dr_wav の drwav_init は1回で約4.5KB使うので、core1 に別途8KBを確保する
bool core1_separate_stack = true;

void setup1() {
    LittleFS.begin();

    // hidden判定を待つ
    const bool hidden = rp2040.fifo.pop();

    for (size_t t = 0; t < kTracks; t++) {
        const char* path = sound_path(t, hidden);
        if (path) load_track(t, path);
    }

    audio_setup();
}

void loop1() { audio_loop(); }