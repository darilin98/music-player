//
// Created by Darek Rudiš on 25.02.2025.
//

#include "decoder.hpp"
#include "player.hpp"
#include "ui_renderer.hpp"
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

using queue_t = std::vector<track_ptr_t>;

class UiController {
public:
    UiController();
    void beginRenderLoop();
private:
    void beginTrackPlayback();
    void stopTrackPlayback();
    void updateFileList();
    void showErrorPopup(const std::string& message) const;
    void processTrackSelection();
    void addTrackToQueue();

    Decoder dec_;
    Player player_;
    UiRenderer ui_;
    std::vector<std::string> files_;
    queue_t track_queue_;
    track_ptr_t current_track_;
    std::string path_;
    int highlight_ = 0;
    std::thread playback_thread_;
    bool playing_ = false;
};
#endif //UICONTROLLER_HPP
