//
// Created by Darek Rudiš on 10.02.2025.
//
#include "rtaudio/RtAudio.h"
#include "rtaudio/rtaudio_c.h"
#include "decoder.hpp"
#include <atomic>

#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
public:
    explicit Player();
    void load_track(const track_ptr_t& track);
    void play_track();
    void raise_volume();
    void lower_volume();
    void pause_track();
    void resume_track();
    [[nodiscard]] bool is_paused() const;
    void stop_track();
    bool is_stopped() const;

private:
    static int audioCallback(void *outputBuffer, void *, unsigned int nFrames,
                             double, RtAudioStreamStatus, void *userData);
    std::atomic<bool> stop_playback_{false};
    bool paused_;
    RtAudio audio_;
    RtAudio::StreamParameters params_;
    unsigned int BUFFER_SIZE = 512;
    unsigned int SAMPLE_RATE = 44100;
    float saved_volume_ = DEFAULT_VOLUME;
    track_ptr_t track_;
};

#endif //PLAYER_HPP
