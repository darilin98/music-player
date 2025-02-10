//
// Created by Darek Rudiš on 04.02.2025.
//
#include "player.hpp"

Player::Player()
{
    if (audio_.getDeviceCount() == 0) {
        throw std::runtime_error("No audio devices found!");
    }

    params_.deviceId = audio_.getDefaultOutputDevice();
    params_.nChannels = 1;
    params_.firstChannel = 0;

}

void Player::load_track(track_ptr_t& track)
{
    track_ = std::move(track);
}

void Player::play_track()
{
    if (track_ == nullptr)
    {
        throw std::runtime_error("No track found!");
    }
    if (audio_.isStreamOpen()) {
        std::cout << "Closing previous audio stream..." << std::endl;
        audio_.closeStream();
    }

    TrackInfo info = track_->getTrackInfo();
    name_t name = info.name;
    SAMPLE_RATE = info.sample_rate;
    params_.nChannels = info.data.channels;
    AudioData audio_data = info.data;

    try {
        audio_.openStream(&params_, nullptr, RTAUDIO_SINT16, SAMPLE_RATE, &BUFFER_SIZE, audioCallback, &audio_data);
        audio_.startStream();
        while (audio_.isStreamRunning()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        audio_.closeStream();
    } catch (RtAudioErrorType &e) {
        if (audio_.isStreamOpen()) audio_.closeStream();
    }
}
    // Audio callback function
int Player::audioCallback(void *outputBuffer, void *, unsigned int nFrames,
                  double, RtAudioStreamStatus, void *userData) {
    auto *audio_data = static_cast<AudioData *>(userData);
    if(!audio_data) return 0;

    int16_t *buffer = static_cast<int16_t *>(outputBuffer);
    size_t channels = audio_data->channels;

    for (unsigned int i = 0; i < nFrames; i++) {
        for (size_t ch = 0; ch < channels; ch++) {
            if (audio_data->current_sample < audio_data->total_samples) {
                buffer[i * channels + ch] = audio_data->pcmData[audio_data->current_sample++];
            } else {
                buffer[i * channels + ch] = 0;
            }
        }
    }
    return 0;
}
