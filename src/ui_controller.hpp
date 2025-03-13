//
// Created by Darek Rudiš on 25.02.2025.
//

#include "decoder.hpp"
#include "player.hpp"
#include <ncurses.h>

#include <filesystem>
#include <thread>

#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

class UiController {
public:
    UiController();
    void beginRenderLoop();
private:
    void beginTrackPlayback(std::thread& playback_thread, const int& highlight, std::vector<std::string>& files, bool& playing, const std::string& path);
    void stopTrackPlayback(std::thread& playback_thread, bool& playing);
    void updateFileList(std::vector<std::string>& files, const std::string& path);
    void renderFileList(const std::vector<std::string>& files, const int& highlight);

    Decoder dec;
    Player player;
};
#endif //UICONTROLLER_HPP
