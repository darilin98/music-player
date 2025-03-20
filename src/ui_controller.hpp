//
// Created by Darek Rudiš on 25.02.2025.
//

#include "decoder.hpp"
#include "player.hpp"
#include <ncurses.h>
#include <queue>
#include <filesystem>
#include <thread>

#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

constexpr char KEY_QUIT = 'q';
constexpr char KEY_PAUSE = ' ';
constexpr char KEY_NEXT_QUEUE = 'n';
constexpr char KEY_ADD_QUEUE = 'a';
constexpr char KEY_PLAY_TRACK = '\n';

using queue_t = std::queue<track_ptr_t>;

class UiController {
public:
    UiController();
    void beginRenderLoop();
private:
    void beginTrackPlayback();
    void stopTrackPlayback();
    void updateFileList();
    void renderFileList();
    void showErrorPopup(const std::string& message);
    void processTrackSelection();

    Decoder dec;
    Player player;
    std::vector<std::string> files;
    queue_t track_queue;
    std::string path;
    int highlight = 0;
    std::thread playback_thread;
    bool playing = false;
    bool running = true;
};
#endif //UICONTROLLER_HPP
