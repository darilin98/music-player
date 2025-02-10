//
// Created by Darek Rudiš on 10.02.2025.
//
#include "decoder.hpp"

using name_t = std::string;

track_ptr_t Decoder::decode_mp3(const name_t& track_name)
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
};