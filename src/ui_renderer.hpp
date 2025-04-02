//
// Created by Darek Rudiš on 01.04.2025.
//

#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

#include <ncurses.h>
#include <string>
#include <vector>
#include <queue>
#include "decoder.hpp"

class UiRenderer {
public:
    UiRenderer();
    void renderFileList(const std::vector<std::string>& files, size_t highlight) const;
    void renderTrackQueue(const std::deque<track_ptr_t>& queue) const;
    void renderStatusBar(const track_ptr_t& current_track, const bool& playing, const bool& paused) const;
    void refreshAll() const;
    void updateAnimationFrame(const bool& playing, const bool& paused) const;

private:
    void renderTrackPlayingText(const MetaData& meta_data) const;

    WINDOW *file_list_win_;
    WINDOW *track_queue_win_;
    WINDOW *status_bar_win_;
};

#endif //UI_RENDERER_HPP
