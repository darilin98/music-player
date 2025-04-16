//
// Created by Darek Rudiš on 25.02.2025.
//
/**
 * @file ui_controller.hpp
 * @brief Defines class and constants regarding user interface core logic
 * @author Darek Rudiš
 * @date 2025-02-25
 */

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
constexpr char KEY_VOLUME_UP= 'u';
constexpr char KEY_VOLUME_DOWN = 'd';

using queue_t = std::deque<track_ptr_t>;
using file_t = std::filesystem::directory_entry;
using file_list_t = std::vector<file_t>;

/**
 * @class UiController
 * @brief Takes care of user input handling and interaction between program components
 */
class UiController {
public:
    /**
     * @brief Initializes ncurses core settings
     */
    UiController();

    /**
     * @warning Alive for the entire lifespan of the program
     * @brief Starts main loop of user interface
     */
    void beginRenderLoop();
private:
    void beginTrackPlayback(const track_ptr_t& track);
    void stopTrackPlayback();
    void updateFileList();
    void showErrorPopup(const std::string& message) const;
    /**
     * @brief Attempt to decode a file and start playback
     */
    void processTrackSelection();
    void addTrackToQueue();
    void processNextTrackFromQueue();

    Decoder dec_; ///< Decoder instance for creating Track objects
    Player player_; ///< Player instance to delegate Track playback
    UiRenderer ui_; ///< Helper renderer instance that separates concern for ncurses rendering
    file_list_t files_; ///< Files in current directory
    queue_t track_queue_; ///< Tracks ready to be played
    track_ptr_t current_track_; ///< Track currently loaded in the player_ instance and playing
    std::string path_; ///< Current working directory
    int highlight_ = 0; ///< Current cursor position
    std::thread playback_thread_; ///< Thread on which the player_ instance operates
    bool playing_ = false;
};
#endif //UICONTROLLER_HPP
