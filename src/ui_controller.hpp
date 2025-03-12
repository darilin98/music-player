//
// Created by Darek Rudiš on 25.02.2025.
//

#include "decoder.hpp"
#include "player.hpp"
#include <ncurses.h>

#include <filesystem>
#include <thread>

#ifndef RENDERER_HPP
#define RENDERER_HPP

class UiController {
public:
    UiController();
    void beginRenderLoop();
private:
    void beginTrackPlayback(std::thread& playback_thread, const int& highlight, std::vector<std::string>& files, bool& playing);
    void stopTrackPlayback(std::thread& playback_thread, bool& playing);
    Decoder dec;
    Player player;
};
#endif //RENDERER_HPP
