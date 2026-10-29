#include <Arduino.h>
#include <LittleFS.h>

#include "i2s_speaker/i2s_speaker.hpp"

namespace {
const char* const kNormalSounds[kTracks] = {"/snare1.wav", "/snare2.wav",
                                            "/hat.wav"};

const char* const kHiddenSounds[kTracks] = {};
}  // namespace

// 既定では8KBのスタックを両コアで4KBずつ分け合う。
// dr_wav の drwav_init は1回で約4.5KB使うので、core1 に別途8KBを確保する
bool core1_separate_stack = true;

void setup1() {
    LittleFS.begin();

    // hidden判定を待つ
    const bool hidden = rp2040.fifo.pop();

    const char* const* sounds = hidden ? kHiddenSounds : kNormalSounds;

    for (size_t t = 0; t < kTracks; t++) {
        if (sounds[t]) load_track(t, sounds[t]);
    }

    audio_setup();
}

void loop1() { audio_loop(); }