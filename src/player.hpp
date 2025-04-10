//
// Created by Darek Rudiš on 10.02.2025.
//

/**
 * @file player.hpp
 * @author Darek Rudiš
 * @date 2025-02-10
 */

#include "rtaudio/RtAudio.h"
#include "rtaudio/rtaudio_c.h"
#include "decoder.hpp"
#include <atomic>

#ifndef PLAYER_HPP
#define PLAYER_HPP

/**
 * @class Player
 * @brief Manages playback of data from a Track instance
 */
class Player {
public:
    /**
    * @brief Constructs a new Player instance and initializes audio system.
    */
    explicit Player();

    /**
     * @brief Redirects local pointer to given Track instance
     * @param track A valid! Track instance
     */
    void load_track(const track_ptr_t& track);

    /**
     * @warning Blocking method
     * @brief Starts track playback and only completes after stop_track() has been called or Track reached the end
     */
    void play_track();
    void raise_volume();
    void lower_volume();
    void pause_track();
    void resume_track();
    [[nodiscard]] bool is_paused() const;

    /**
     * @brief Only way to abort the execution of play_track() method from outside of Player class
     */
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
    unsigned int SAMPLE_RATE = 44100; ///< Audio sample rate in Hz
    float saved_volume_ = DEFAULT_VOLUME; ///< Saves volume modifier when switching between tracks
    track_ptr_t track_; ///< Current Track in playback or ready to be played
};

#endif //PLAYER_HPP
