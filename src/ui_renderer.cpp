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
        mvwprintw(track_queue_win_, i + 1, 2, "%s", queue[i]->getTrackInfo().meta_data.track_name.c_str());
    }
    wrefresh(track_queue_win_);
}
void UiRenderer::renderStatusBar(const track_ptr_t& current_track, const bool& playing, const bool& paused) const
{
    wclear(status_bar_win_);
    box(status_bar_win_, 0, 0);
    if (playing) {
        mvwprintw(status_bar_win_, 0, 2, "Now Playing: ");
        mvwprintw(status_bar_win_, 0, 15, "   ");
        if (paused)
        {
            wattron(status_bar_win_, A_BOLD);
            mvwprintw(status_bar_win_, 2, 2, "||");
        } else {
            mvwprintw(status_bar_win_, 2, 2, "|>");
        }
        if (current_track != nullptr)
        {
            MetaData metaData = current_track->getTrackInfo().meta_data;
            renderTrackPlayingText(metaData);
        }
        wattroff(status_bar_win_, A_BOLD);
    }
    else
        mvwprintw(status_bar_win_, 0, 2, "Select a track and press ENTER to play");
    mvwprintw(status_bar_win_, 4, 2 , "[Q] Quit  [SPACE] Pause  [A] Add  [N] Next");
    wrefresh(status_bar_win_);
}
void UiRenderer::renderTrackPlayingText(const MetaData& metaData) const
{
    wattron(status_bar_win_, A_BOLD);
    mvwprintw(status_bar_win_, 2, 6, "%s", metaData.track_name.c_str());
    wattroff(status_bar_win_, A_BOLD);
    mvwprintw(status_bar_win_, 2, 6 + metaData.track_name.length(), " by: ");
    wattron(status_bar_win_, A_BOLD);
    mvwprintw(status_bar_win_, 2, 11 + metaData.track_name.length(), "%s", metaData.artist.c_str());
    wattroff(status_bar_win_, A_BOLD);
    mvwprintw(status_bar_win_, 2, 12 + metaData.track_name.length() + metaData.artist.length(), " on: ");
    wattron(status_bar_win_, A_BOLD);
    mvwprintw(status_bar_win_, 2, 17 + metaData.track_name.length() + metaData.artist.length(), "%s", metaData.album.c_str());
    wattroff(status_bar_win_, A_BOLD);
}
void UiRenderer::updateAnimationFrame(const bool& playing, const bool& paused) const
{
    static const char *frames[] = {
        ".!||!.!|||!.", "!|!||!.!||!.", "!|!!|!.!|!..", "||.!||!.!!..", "|!..!||!!.!.", "!....!|!..!|", "..!.!...!!.!", "..!!...!|!..", ".!|!...!||!."
    };
    static int frame = 0;
    static int frame_counter = 0;
    const int frame_delay = 5;
    if (playing)
    {
        if (!paused)
        {
            frame_counter++;
            if (frame_counter >= frame_delay) {
                frame_counter = 0;
                frame = (frame + 1) % (sizeof(frames) / sizeof(frames[0]));
            }
        }

        mvwprintw(status_bar_win_, 0, 15, frames[frame]);
        wrefresh(status_bar_win_);
    }
}
void UiRenderer::refreshAll() const
{
    wrefresh(file_list_win_);
    wrefresh(track_queue_win_);
    wrefresh(status_bar_win_);
}




