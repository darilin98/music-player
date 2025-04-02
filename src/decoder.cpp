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
        return std::make_shared<ErrorTrack>("Failed to open file");
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
        return std::make_shared<ErrorTrack>("Failed to open mp3 file");
    }

    size_t pcmBufferSize = mp3dec.samples * sizeof(int16_t);
    int16_t* pcmData = (int16_t*)malloc(pcmBufferSize);
    if(!pcmData)
    {
        mp3dec_ex_close(&mp3dec);
        free(mp3_data);
        return std::make_shared<ErrorTrack>("Failed to allocate memory");
    }

    size_t samplesRead = mp3dec_ex_read(&mp3dec, pcmData, mp3dec.samples);
    if (samplesRead == 0)
    {
        free(pcmData);
        mp3dec_ex_close(&mp3dec);
        free(mp3_data);
        return std::make_shared<ErrorTrack>("No samples decoded");
    }

    AudioData audioData = {pcmData, samplesRead, 0, mp3dec.info.channels};

    // GET METADATA
    MetaData metaData = parse_id3v1(track_name);
    metaData.duration = audioData.total_samples / (mp3dec.info.hz * audioData.channels);

    return std::make_shared<MP3Track>(metaData, audioData, mp3dec.info.hz);
};

MetaData Decoder::parse_id3v1(const std::string& filename) {
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f) {
        perror("Failed to open file");
        return {"Unknown", "Unknown Artist", "Unknown", 0};
    }

    // Seek to the last 128 bytes - ID3v1 location
    if (fseek(f, -128, SEEK_END) != 0) {
        fclose(f);
        return {"Unknown", "Unknown Artist", "Unknown", 0};
    }

    char tag[128] = {0};
    if (fread(tag, 1, 128, f) != 128) {
        fclose(f);
        return {"Unknown", "Unknown Artist", "Unknown", 0};
    }
    fclose(f);

    // Validate Tag
    if (strncmp(tag, "TAG", 3) != 0) {
        return {"Unknown", "Unknown Artist", "Unknown", 0};
    }

    MetaData meta;
    meta.track_name = std::string(tag + 3, 30);
    meta.artist = std::string(tag + 33, 30);
    meta.album = std::string(tag + 63, 30);

    meta.track_name.erase(meta.track_name.find_last_not_of('\0') + 1);
    meta.artist.erase(meta.artist.find_last_not_of('\0') + 1);
    meta.album.erase(meta.album.find_last_not_of('\0') + 1);

    return meta;
}