#define DR_WAV_IMPLEMENTATION

#include "wav.hpp"

#include <LittleFS.h>
#include <dr_wav.h>

bool WavFile::open(const char* path) {
    close();

    file_ = LittleFS.open(path, "r");

    if (!file_) return false;

    if (!drwav_init(&wav_, onRead, onSeek, onTell, &file_, nullptr)) {
        file_.close();
        return false;
    }

    opened_ = true;
    return true;
}

void WavFile::close() {
    if (opened_) {
        drwav_uninit(&wav_);
        opened_ = false;
    }

    file_.close();
}

size_t WavFile::onRead(void* user, void* buf, size_t len) {
    return static_cast<File*>(user)->read(static_cast<uint8_t*>(buf), len);
}

drwav_bool32 WavFile::onSeek(void* user, int offset, drwav_seek_origin origin) {
    File* f = static_cast<File*>(user);
    int64_t pos = offset;

    if (origin == DRWAV_SEEK_CUR) {
        pos += f->position();
    } else if (origin == DRWAV_SEEK_END) {
        pos += f->size();
    }

    if (pos < 0) return DRWAV_FALSE;

    return f->seek(static_cast<uint32_t>(pos), SeekSet) ? DRWAV_TRUE
                                                        : DRWAV_FALSE;
}

drwav_bool32 WavFile::onTell(void* user, drwav_int64* cursor) {
    *cursor = static_cast<File*>(user)->position();
    return DRWAV_TRUE;
}