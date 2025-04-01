//
// Created by Darek Rudiš on 01.04.2025.
//
#include "ui_renderer.hpp"

UiRenderer::UiRenderer()
{
    int mid_x = COLS / 2;
    file_list_win_ = newwin(LINES - 5, mid_x, 0, 0);
    track_queue_win_ = newwin(LINES - 5, COLS - mid_x, 0, mid_x);
    status_bar_win_ = newwin(5, COLS, LINES - 5, 0);
}


void UiRenderer::renderFileList(const std::vector<std::string> &files, size_t highlight) const
{
    wclear(file_list_win_);
    box(file_list_win_, 0, 0);
    for (size_t i = 0; i < files.size(); ++i) {
        if (i == highlight) {
            wattron(file_list_win_, A_REVERSE);
        }
        mvwprintw(file_list_win_, i + 1, 2, files[i].c_str());
        if (i == highlight) {
            wattroff(file_list_win_, A_REVERSE);
        }
    }
    wrefresh(file_list_win_);
}
void UiRenderer::renderTrackQueue(const std::vector<track_ptr_t> &queue) const
{
    wclear(track_queue_win_);
    box(track_queue_win_, 0, 0);
    mvwprintw(track_queue_win_, 0, 2, "Track Queue");
    for(size_t i = 0; i < queue.size(); ++i)
    {
        mvwprintw(track_queue_win_, i + 1, 2, "sample_text");
    }
    wrefresh(track_queue_win_);
}
void UiRenderer::renderStatusBar(const track_ptr_t& current_track, bool playing) const
{
    wclear(status_bar_win_);
    box(status_bar_win_, 0, 0);
    if (playing)
        mvwprintw(status_bar_win_, 0, 2, "Now Playing: ");
    else
        mvwprintw(status_bar_win_, 0, 2, "Select a track and press ENTER to play");
    mvwprintw(status_bar_win_, 4, 2 , "[Q] Quit  [SPACE] Pause  [A] Add  [N] Next");
    wrefresh(status_bar_win_);
}
void UiRenderer::refreshAll() const
{
    wrefresh(file_list_win_);
    wrefresh(track_queue_win_);
    wrefresh(status_bar_win_);
}




