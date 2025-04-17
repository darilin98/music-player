//
// Created by Darek Rudiš on 01.04.2025.
//
#include "ui_renderer.hpp"

UiRenderer::UiRenderer()
{
    int mid_x = COLS / 2;
    file_list_win_ = newwin(LINES - 6, mid_x, 0, 0);
    track_queue_win_ = newwin(LINES - 6, COLS - mid_x, 0, mid_x);
    status_bar_win_ = newwin(6, COLS, LINES - 6, 0);
}


void UiRenderer::renderFileList(const file_list_t& files, size_t highlight) const
{
    werase(file_list_win_);
    box(file_list_win_, 0, 0);
    for (size_t i = 0; i < files.size(); ++i) {
        if (i == highlight)
            wattron(file_list_win_, A_REVERSE);
        if (std::filesystem::path(files[i]).extension() == ".mp3")
            wattron(file_list_win_, COLOR_PAIR(1));

        if (is_directory(files[i]))
            mvwprintw(file_list_win_, i + 1, 2, "/%s", files[i].path().filename().c_str());
        else
            mvwprintw(file_list_win_, i + 1, 2, files[i].path().filename().c_str());

        if (std::filesystem::path(files[i]).extension() == ".mp3")
            wattroff(file_list_win_, COLOR_PAIR(1));
        if (i == highlight)
            wattroff(file_list_win_, A_REVERSE);
    }
    wnoutrefresh(file_list_win_);
}
void UiRenderer::renderTrackQueue(const std::deque<track_ptr_t> &queue) const
{
    werase(track_queue_win_);
    box(track_queue_win_, 0, 0);
    mvwprintw(track_queue_win_, 0, 2, "Track Queue");
    for(size_t i = 0; i < queue.size(); ++i)
    {
        mvwprintw(track_queue_win_, i + 1, 2, "%s", queue[i]->getTrackInfo().meta_data.track_name.c_str());
    }
    wnoutrefresh(track_queue_win_);
}
void UiRenderer::renderStatusBar(const track_ptr_t& current_track, const bool& playing, const bool& paused) const
{
    werase(status_bar_win_);
    box(status_bar_win_, 0, 0);
    if (playing) {
        mvwprintw(status_bar_win_, 0, 2, "Now Playing: ");
        mvwprintw(status_bar_win_, 0, 15, "   ");
        if (paused)
        {
            wattron(status_bar_win_, A_BOLD);
            mvwprintw(status_bar_win_, 2, 2, "||");
        } else {
            wattron(status_bar_win_, A_BOLD);
            mvwprintw(status_bar_win_, 2, 2, "|>");
        }
        if (current_track != nullptr)
        {
            TrackInfo track_info = current_track->getTrackInfo();
            renderTrackPlayingText(track_info.meta_data);
        }
        wattroff(status_bar_win_, A_BOLD);
    }
    else
        mvwprintw(status_bar_win_, 0, 2, "Select a track and press ENTER to play");
    mvwprintw(status_bar_win_, 5, 2 , "[Q] Quit  [SPACE] Pause  [A] Add  [N] Next  [U/D] Volume Control");
    wnoutrefresh(status_bar_win_);
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
void UiRenderer::updateAnimationFrame(const track_ptr_t& current_track, const bool& playing, const bool& paused) const
{
    static const char *frames[] = {
        ".!||!.!|||!.", "!|!||!.!||!.", "!|!!|!.!|!..", "||.!||!.!!..", "|!..!||!!.!.", "!....!|!..!|", "..!.!...!!.!", "..!!...!|!..", ".!|!...!||!."
    };
    static size_t frame = 0;
    static size_t frame_counter = 0;
    if (playing)
    {
        if (!paused)
        {
            frame_counter++;
            if (frame_counter >= frame_delay) {
                frame_counter = 0;
                frame = (frame + 1) % std::size(frames);
            }
        }
        if (current_track != nullptr)
        {
            TrackInfo track_info = current_track->getTrackInfo();
            uint32_t elapsed_time_minutes = track_info.data.current_sample / track_info.sample_rate / track_info.data.channels / 60;
            uint32_t elapsed_time_seconds = track_info.data.current_sample / track_info.data.channels / track_info.sample_rate % 60;
            uint32_t total_minutes = track_info.meta_data.duration / 60;
            uint32_t total_seconds = track_info.meta_data.duration % 60;
            mvwprintw(status_bar_win_, 3, 6, "%d:%02d / %d:%02d",elapsed_time_minutes, elapsed_time_seconds, total_minutes, total_seconds);
        }
        mvwprintw(status_bar_win_, 0, 15, frames[frame]);
        wrefresh(status_bar_win_);
    }
}
void UiRenderer::refreshAll() const
{
    wnoutrefresh(file_list_win_);
    wnoutrefresh(track_queue_win_);
    wnoutrefresh(status_bar_win_);
    doupdate();
}




