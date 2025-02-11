#include <iostream>

#include "decoder.hpp"
#include "player.hpp"
#include <ncurses.h>


int main(int argc, char *argv[]) {

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    name_t track_name = argv[1];
    Decoder dec;
    track_ptr_t track = dec.decode_mp3(track_name);
    if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
    {
        std::cerr << "Error decoding audio file!" << std::endl;
        return 1;
    }

    Player player;
    player.load_track(track);
    player.play_track();

    return 0;
}
