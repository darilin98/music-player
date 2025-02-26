//
// Created by Darek Rudiš on 25.02.2025.
//
#include "ui_controller.hpp"

UiController::UiController()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
}

void UiController::beginRenderLoop()
{

    std::vector<std::string> files;
    std::string path = ".";

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        files.push_back(entry.path().filename().string());
    }

    int highlight = 0;
    int ch;
    std::thread playback_thread;
    bool playing = false;

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
            if (playing && playback_thread.joinable())
            {
                continue;
            }
            name_t& track_name = files[highlight];
            track_ptr_t track = dec.decode_mp3(track_name);
            if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
            {
                std::cerr << "Error decoding audio file!" << std::endl;
            }
            player.load_track(track);

            playing = true;
            playback_thread = std::thread([this, &playing]() {
                player.play_track();
                playing = false;
            });
        } else if (ch == ' ')
        {
            if (playing) {
                if (player.is_paused()) {
                    player.resume_track();
                } else {
                    player.pause_track();
                }
            }
        }
        else if (ch == 'q') {
            /*
            if (playing && playback_thread.joinable()) {
                playback_thread.join();
            }
            */
            playing = false;
            break; // Exit on 'q'
        }
    }
    endwin();
}
