//
// Created by Darek Rudiš on 10.02.2025.
//
#include "rtaudio/RtAudio.h"
#include "rtaudio/rtaudio_c.h"
#include "decoder.hpp"

#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
public:
    explicit Player();
    void load_track(track_ptr_t& track);
    void play_track();
    void pause_track();
    void resume_track();
    [[nodiscard]] bool is_paused() const;

private:
    static int audioCallback(void *outputBuffer, void *, unsigned int nFrames,
                             double, RtAudioStreamStatus, void *userData);
    bool paused_;
    RtAudio audio_;
    RtAudio::StreamParameters params_;
    unsigned int BUFFER_SIZE = 512;
    unsigned int SAMPLE_RATE = 44100;
    track_ptr_t track_;
};

#endif //PLAYER_HPP
