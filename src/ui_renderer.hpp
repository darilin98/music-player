//
// Created by Darek Rudiš on 01.04.2025.
//

#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

#include <ncurses.h>
#include <string>
#include <vector>

class UiRenderer {
public:
    UiRenderer();
    void renderFileList(const std::vector<std::string>& files, size_t highlight) const;
    void renderTrackQueue(const std::vector<std::string>& queue) const;
    void renderStatusBar(const std::string& current_track, bool playing) const;
    void refreshAll() const;

private:
    WINDOW *file_list_win_;
    WINDOW *track_queue_win_;
    WINDOW *status_bar_win_;
};

#endif //UI_RENDERER_HPP
