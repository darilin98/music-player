#include <iostream>

#include <cmath>
#include <thread>

#include "rtaudio/RtAudio.h"
#include "rtaudio/rtaudio_c.h"
#include "minimp3/minimp3.h"
#include "minimp3/minimp3_ex.h"

using name_t = std::string;
struct AudioData {
    int16_t* pcmData;
    size_t total_samples;
    size_t current_sample;
    int channels;
};

struct TrackInfo {
    name_t name;
    AudioData data;
};

class GenericTrack {
    public:
    virtual ~GenericTrack() = 0;
    virtual TrackInfo getTrackInfo() const = 0;
};

class MP3Track : public GenericTrack {
public:
    explicit MP3Track(const name_t& name, const AudioData& data)
        : name_(name), audio_data_(data){}
    TrackInfo getTrackInfo() const override { return TrackInfo {name_, audio_data_};}
private:
    name_t name_;
    AudioData audio_data_;
};

class ErrorTrack : public GenericTrack {
public:
    explicit ErrorTrack(const name_t& message)
        : error_message_(message) {}
    TrackInfo getTrackInfo() const override { return TrackInfo {error_message_, AudioData()};}

private:
    name_t error_message_;
};

using track_ptr_t = std::unique_ptr<GenericTrack>;
class Decoder {
public:
    track_ptr_t decode_mp3(const name_t& track_name)
    {
        FILE *f = fopen(track_name.c_str(), "rb");
        if (!f)
        {
            return std::make_unique<ErrorTrack>("Failed to open file");
        }
        fseek(f, 0, SEEK_END);
        size_t filesize = ftell(f);
        fseek(f, 0, SEEK_SET);

        uint8_t* mp3_data = (uint8_t*)malloc(filesize);
        fread(mp3_data, filesize, 1, f);
        fclose(f);

        mp3dec_ex_t mp3dec;
        if (mp3dec_ex_open_buf(&mp3dec, mp3_data, filesize, MP3D_SEEK_TO_SAMPLE))
        {
            free(mp3_data);
            return std::make_unique<ErrorTrack>("Failed to open mp3 file");
        }

        size_t pcmBufferSize = mp3dec.samples * sizeof(int16_t);
        int16_t* pcmData = (int16_t*)malloc(pcmBufferSize);
        if(!pcmData)
        {
            mp3dec_ex_close(&mp3dec);
            free(mp3_data);
            return std::make_unique<ErrorTrack>("Failed to allocate memory");
        }

        size_t samplesRead = mp3dec_ex_read(&mp3dec, pcmData, mp3dec.samples);
        if (samplesRead == 0)
        {
            free(pcmData);
            mp3dec_ex_close(&mp3dec);
            free(mp3_data);
            return std::make_unique<ErrorTrack>("No samples decoded");
        }

        AudioData audioData = {pcmData, samplesRead, 0, mp3dec.info.channels};

        // GET METADATA

        return std::make_unique<MP3Track>(track_name, audioData);
    }
};


class Player {
public:
    explicit Player()
    {
        if (audio_.getDeviceCount() == 0) {
            throw std::runtime_error("No audio devices found!");
        }

        params_.deviceId = audio_.getDefaultOutputDevice();
        params_.nChannels = 1;
        params_.firstChannel = 0;
    }
    void load_track(track_ptr_t& track)
    {
        track_ = std::move(track);
    }
    void play_track()
    {
        if (track_ == nullptr)
        {
            throw std::runtime_error("No track found!");
        }

        TrackInfo info = track_->getTrackInfo();
        name_t name = info.name;
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
    static int audioCallback(void *outputBuffer, void *, unsigned int nFrames,
                      double, RtAudioStreamStatus, void *userData) {
        auto *audio_data = static_cast<AudioData *>(userData);
        if(!audio_data) return 0;

        int16_t *buffer = static_cast<int16_t *>(outputBuffer);

        for (unsigned int i = 0; i < nFrames; i++)
        {
            if (audio_data->current_sample < audio_data->total_samples)
            {
                buffer[i] = audio_data->pcmData[audio_data->current_sample++];
            } else
            {
                buffer[i] = 0;
            }
        }
        return 0;
    }
private:

    RtAudio audio_;
    RtAudio::StreamParameters params_;
    unsigned int BUFFER_SIZE = 512;
    unsigned int SAMPLE_RATE = 44100;
    track_ptr_t track_;
};


int main(int argc, char *argv[]) {

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    name_t track_name = argv[1];
    Decoder dec;
    track_ptr_t track = dec.decode_mp3(track_name);
    if (typeid(*track) == typeid(ErrorTrack))
    {
        std::cerr << "Error decoding audio file!" << std::endl;
        return 1;
    }

    Player player;
    player.load_track(track);
    player.play_track();

    return 0;
}
