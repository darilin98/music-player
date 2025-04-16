//
// Created by Darek Rudiš on 01.04.2025.
//
/**
 * @file ui_renderer.hpp
 * @brief Defines class handling UI window rendering specifics
 * @author Darek Rudiš
 * @date 2025-04-01
 */
#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

#include <ncurses.h>
#include <string>
#include <vector>
#include <queue>
#include <filesystem>
#include <memory>
#include "decoder.hpp"

using file_t = std::filesystem::directory_entry;
using file_list_t = std::vector<file_t>;

/**
 * @class UiRenderer
 * @brief Renders updated UI state based on data received from controller class
 */
class UiRenderer {
public:
    /**
     * @brief Sets up ncurses window sizes based on the current terminal window size
     */
    UiRenderer();

    /**
     * @brief Refreshes file_list_win_ with provided file data
     * @param files A list of files in the current directory
     * @param highlight Current cursor position
     */
    void renderFileList(const file_list_t& files, size_t highlight) const;
    /**
     * @brief Refreshes track_queue_win_
     * @param queue List of tracks that are queued to be played
     */
    void renderTrackQueue(const std::deque<track_ptr_t>& queue) const;

    /**
     * @brief Renders status bar in its stationary state
     * @param current_track Track that provides metadata to be displayed
     * @param playing
     * @param paused
     */
    void renderStatusBar(const track_ptr_t& current_track, const bool& playing, const bool& paused) const;
    void refreshAll() const;

    /**
     * @brief Updates animations in the status bar window
     * @warning Needs to be updated frequently to achieve fluid movement
     * @param current_track Track to update elapsed time counter
     * @param playing Flag between stationary and updating
     * @param paused Decide whether playing animation should be paused
     */
    void updateAnimationFrame( const track_ptr_t& current_track,const bool& playing, const bool& paused) const;

private:
    void renderTrackPlayingText(const MetaData& meta_data) const;
    static constexpr int frame_delay = 5;
    /// Main ncurses windows that form the whole app
    WINDOW *file_list_win_; ///< File browser window
    WINDOW *track_queue_win_; ///< List of Queued Tracks
    WINDOW *status_bar_win_; ///< Bottom status bar which displays state information
};

#endif //UI_RENDERER_HPP
