//
// Created by Darek Rudiš on 25.02.2025.
//
#include "ui_controller.hpp"

UiController::UiController()
{
    initscr();
    refresh();
    napms(1000);
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(0);
    // Explicit initialization of the renderer after all setup ncurses functions have been called
    ui_ = UiRenderer();
}

void UiController::beginRenderLoop()
{
    path_ = std::filesystem::current_path().string();

    updateFileList();

    bool running = true;
    while (running)
    {
        ui_.renderFileList(files_, highlight_);
        ui_.renderTrackQueue(track_queue_);
        ui_.renderStatusBar(current_track_, playing_);
        switch (int pressed_key = getch()) {
            case KEY_UP:
                if (highlight_ > 0) highlight_--;
                break;
            case KEY_DOWN:
                if (highlight_ < files_.size() - 1) highlight_++;
                break;
            case KEY_PLAY_TRACK:
                processTrackSelection();
                break;
            case KEY_PAUSE:
                if (playing_) {
                    player_.is_paused() ? player_.resume_track() : player_.pause_track();
                }
                break;
            case KEY_QUIT:
                stopTrackPlayback();
                running = false;
                break;
        }
    }
    endwin();
}

void UiController::beginTrackPlayback()
{
    name_t track_name = path_ + "/" + files_[highlight_];
    track_ptr_t track = dec_.decode_mp3(track_name);
    if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
    {
        throw std::runtime_error("Error decoding track!");
    }
    player_.load_track(track);

    playing_ = true;
    playback_thread_ = std::thread([this]() {
        player_.play_track();
        playing_ = false;
    });
}
void UiController::stopTrackPlayback()
{
    if (playing_)
    {
        player_.stop_track();
        if (playback_thread_.joinable())
            playback_thread_.join();
        playing_ = false;
    }
}

void UiController::updateFileList()
{
    files_.clear();
    files_.emplace_back(".."); // For going up a directory
    for (const auto& entry : std::filesystem::directory_iterator(path_))
    {
        files_.push_back(entry.path().filename().string());
    }
}

void UiController::showErrorPopup(const std::string &message)
{
    size_t height = 3;
    size_t width = message.size() + 4;
    size_t starty = (LINES - height) / 2;
    size_t startx = (COLS - width) / 2;

    WINDOW* popup = newwin(height, width, starty, startx);
    box(popup, 0, 0);
    mvwprintw(popup, 1, 2, "%s", message.c_str());
    wrefresh(popup);

    napms(1500);

    delwin(popup);
    clear();
    refresh();
}

void UiController::processTrackSelection()
{
    name_t selected = files_[highlight_];
    name_t new_path = (selected == "..") ? std::filesystem::path(path_).parent_path().string() : path_ + "/" + selected;
    if (std::filesystem::is_directory(new_path))
    {
        path_ = new_path;
        highlight_ = 0;
        updateFileList();
    } else
    {
        if (playing_)
            stopTrackPlayback();
        try
        {
            beginTrackPlayback();
        } catch (const std::exception& e)
        {
            showErrorPopup("Error opening file!");
        }
    }
}





