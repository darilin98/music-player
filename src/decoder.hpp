//
// Created by Darek Rudiš on 10.02.2025.
//
/**
 * @file decoder.hpp
 * @brief Contains classes and structures for decoding MP3 files into PCM audio data.
 * @author Darek Rudiš
 * @date 2025-02-10
 */

#include <iostream>

#include <cmath>
#include <thread>
#include <memory>
#include <filesystem>
#include <cstring>

#include "minimp3/minimp3.h"
#include "minimp3/minimp3_ex.h"

#ifndef DECODER_HPP
#define DECODER_HPP

constexpr float DEFAULT_VOLUME = 0.5f;
using name_t = std::string;

/**
 * @struct AudioData
 * @brief Contains PCM audio samples and playback-related data.
 */
struct AudioData {
    int16_t* pcmData; ///< Raw track PCM data
    size_t total_samples;
    size_t current_sample; ///< Current position during playback
    int channels;
    float volume = DEFAULT_VOLUME;
};

/**
 * @struct MetaData
 * @brief Holds data related to track context
 */
struct MetaData {
    name_t track_name;
    name_t artist;
    name_t album;
    uint32_t duration;
};

/**
 * @struct TrackInfo
 * @brief Couples together playback and context information for a specific track
 */
struct TrackInfo {
    MetaData meta_data;
    AudioData data;
    uint32_t sample_rate;
};

class GenericTrack;
using track_ptr_t = std::shared_ptr<GenericTrack>;

/**
 * @class Decoder
 * @brief Transforms valid file into PCM data and all necessary parameters for audio playback
 */
class Decoder {
public:
    /**
     * @brief Attempts to decode a file as an MP3
     * @param track_name Path to target file
     * @return Pointer to a Track object, if decoding failed target is an ErrorTrack
     */
    static track_ptr_t decode_mp3(const name_t& track_name);
private:
    /**
     * @brief Parses ID3v1 metadata from the file.
     * @param file_name Path to the MP3 file.
     * @return MetaData structure containing track info.
     */
    static MetaData parse_id3v1(const name_t& file_name);
};

/**
 * @class GenericTrack
 * @brief Abstract base class for all track types.
 */
class GenericTrack {
public:
    virtual ~GenericTrack() = default;
    /**
     * @brief Returns a copy of the track’s metadata and audio data.
     */
    virtual TrackInfo getTrackInfo() const = 0;
    /**
     * @brief Returns a reference to actual AudioData for tracking playback progress
     */
    virtual AudioData& getAudioDataRef() = 0;
    virtual void setCurrentSample(const size_t& position) = 0;
};

class MP3Track : public GenericTrack {
public:
    explicit MP3Track(const MetaData& meta_data, const AudioData& data, const unsigned int sample_rate)
        : meta_data_(meta_data), audio_data_(data), sample_rate_(sample_rate) {}
    TrackInfo getTrackInfo() const override { return TrackInfo {meta_data_, audio_data_, sample_rate_};}
    AudioData& getAudioDataRef() override { return audio_data_; }
    void setCurrentSample(const size_t& position) override { audio_data_.current_sample = position; }
private:
    MetaData meta_data_;
    AudioData audio_data_;
    unsigned int sample_rate_;
};

class ErrorTrack : public GenericTrack {
public:
    explicit ErrorTrack(const name_t& message)
        : error_message_(message) {}
    TrackInfo getTrackInfo() const override { return TrackInfo {error_message_, "", "",0, AudioData()};}
    AudioData& getAudioDataRef() override { return error_data_; }
    void setCurrentSample(const size_t& position) override {}
private:
    AudioData error_data_ = AudioData();
    name_t error_message_;
};


#endif //DECODER_HPP
