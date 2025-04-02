//
// Created by Darek Rudiš on 10.02.2025.
//

#include <iostream>

#include <cmath>
#include <thread>

#include "minimp3/minimp3.h"
#include "minimp3/minimp3_ex.h"

#ifndef DECODER_HPP
#define DECODER_HPP

using name_t = std::string;
struct AudioData {
    int16_t* pcmData;
    size_t total_samples;
    size_t current_sample;
    int channels;
};
struct MetaData {
    name_t track_name;
    name_t artist;
    name_t album;
    double duration;
};
struct TrackInfo {
    MetaData meta_data;
    AudioData data;
    uint32_t sample_rate;
};
class GenericTrack;
using track_ptr_t = std::shared_ptr<GenericTrack>;

class GenericTrack {
public:
    virtual ~GenericTrack() = default;
    virtual TrackInfo getTrackInfo() const = 0;
    virtual void setCurrentSample(const size_t& position) = 0;
};

class Decoder {
public:
    static track_ptr_t decode_mp3(const name_t& track_name);
private:
    static MetaData parse_id3v1(const name_t& file_name);
};

class MP3Track : public GenericTrack {
public:
    explicit MP3Track(const MetaData& meta_data, const AudioData& data, const unsigned int sample_rate)
        : meta_data_(meta_data), audio_data_(data), sample_rate_(sample_rate) {}
    TrackInfo getTrackInfo() const override { return TrackInfo {meta_data_, audio_data_, sample_rate_};}
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
    void setCurrentSample(const size_t& position) override {}
private:
    name_t error_message_;
};


#endif //DECODER_HPP
