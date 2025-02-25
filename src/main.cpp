#include <iostream>

#include "decoder.hpp"
#include "player.hpp"
#include <ncurses.h>

#include <filesystem>

int main(int argc, char *argv[]) {

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    // name_t track_name = argv[1];
    Decoder dec;

    Player player;

    name_t track_name = argv[1];
    track_ptr_t track = dec.decode_mp3(track_name);
    if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
    {
        std::cerr << "Error decoding audio file!" << std::endl;
        return 1;
    }
    player.load_track(track);
    player.play_track();
    /*

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    std::vector<std::string> files;
    std::string path = ".";

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        files.push_back(entry.path().filename().string());
    }

    int highlight = 0;
    int ch;

    while (true)
    {
        clear();
        for (size_t i = 0; i < files.size(); ++i) {
            if (i == highlight) {
                attron(A_REVERSE); // Highlight selected file
            }
            mvprintw(i, 0, files[i].c_str());
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }
        refresh();

        ch = getch();
        if (ch == KEY_UP && highlight > 0) {
            highlight--;
        } else if (ch == KEY_DOWN && highlight < files.size() - 1) {
            highlight++;
        } else if (ch == '\n') {
            name_t track_name = files[highlight];
            track_ptr_t track = dec.decode_mp3(track_name);
            if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
            {
                std::cerr << "Error decoding audio file!" << std::endl;
                return 1;
            }
            player.load_track(track);
            player.play_track();
        } else if (ch == 'q') {
            break; // Exit on 'q'
        }
    }
    endwin();

    */

    return 0;
}
