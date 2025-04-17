//
// Created by Darek Rudiš on 25.02.2025.
//
#include "ui_controller.hpp"

UiController::UiController()
{
    initscr();
    refresh();
    noecho();
    cbreak();
    keypad(stdscr, true);
    set_escdelay(25);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    curs_set(0);
    // Explicit initialization of the renderer after all setup ncurses functions have been called
    ui_ = UiRenderer();
}

void UiController::beginRenderLoop()
{
    path_ = std::filesystem::current_path().string();

    updateFileList();
    nodelay(stdscr, TRUE);
    ui_.renderFileList(files_, highlight_);
    ui_.renderTrackQueue(track_queue_);
    ui_.renderStatusBar(current_track_, playing_, player_.is_paused());

    bool running = true;
    while (running)
    {
        ui_.detectResize();
        ui_.renderFileList(files_, highlight_);
        ui_.renderTrackQueue(track_queue_);
        ui_.renderStatusBar(current_track_, playing_, player_.is_paused());
        ui_.updateAnimationFrame(current_track_, playing_, player_.is_paused());
        napms(50);
        if (!playing_ && !track_queue_.empty())
        {
            processNextTrackFromQueue();
        }

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
            case KEY_ADD_QUEUE:
                addTrackToQueue();
                break;
            case KEY_PAUSE:
                if (playing_) {
                    player_.is_paused() ? player_.resume_track() : player_.pause_track();
                }
                break;
            case KEY_NEXT_QUEUE:
                processNextTrackFromQueue();
                break;
            case KEY_VOLUME_UP:
                player_.raise_volume();
                break;
            case KEY_VOLUME_DOWN:
                player_.lower_volume();
                break;
            case KEY_QUIT:
                stopTrackPlayback();
                running = false;
                break;
        }
        ui_.refreshAll();
    }
    endwin();
}

void UiController::beginTrackPlayback(const track_ptr_t& track)
{
    if (playback_thread_.joinable())
    {
        playback_thread_.join();
    }
    player_.load_track(track);
    playing_ = true;
    current_track_ = track;
    playback_thread_ = std::thread([this]() {
        player_.play_track();
        playing_ = false;
        current_track_ = nullptr;
        ui_.renderStatusBar(current_track_, playing_, player_.is_paused());
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
        current_track_ = nullptr;
    } else {
        if (playback_thread_.joinable())
            playback_thread_.join();
    }
}

void UiController::updateFileList()
{
    files_.clear();
    files_.emplace_back(std::filesystem::directory_entry(std::filesystem::path(path_) / ".."));
    for (const auto& entry : std::filesystem::directory_iterator(path_))
    {
        files_.push_back(entry);
    }
}

void UiController::showErrorPopup(const std::string &message) const
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
    ui_.renderFileList(files_, highlight_);
    ui_.renderTrackQueue(track_queue_);
    ui_.renderStatusBar(current_track_, playing_, player_.is_paused());
}

void UiController::processTrackSelection()
{
    auto& selected = files_[highlight_];
    std::filesystem::path selected_path = selected.path();

    if (selected_path.filename() == "..") {
        path_ = std::filesystem::path(path_).parent_path().string();
        highlight_ = 0;
        updateFileList();
    } else if (is_directory(selected_path)) {
        path_ = selected_path.string();
        highlight_ = 0;
        updateFileList();
    } else
    {
        if (playing_)
            stopTrackPlayback();
        track_ptr_t track = dec_.decode_mp3(selected_path.string());
        if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr){
            //Check type of returned track
            showErrorPopup("Error opening file!");
            return;
        }
        beginTrackPlayback(track);
    }
    ui_.renderStatusBar(current_track_, playing_, player_.is_paused());
    ui_.renderFileList(files_, highlight_);
}

void UiController::addTrackToQueue()
{
    name_t track_path = files_[highlight_].path().string();
    track_ptr_t track = dec_.decode_mp3(track_path);
    if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
    {
        showErrorPopup("Error opening file!");
        return;
    }
    track_queue_.emplace_back(track);
    ui_.renderTrackQueue(track_queue_);
}

void UiController::processNextTrackFromQueue()
{
    if (playing_)
        stopTrackPlayback();
    if (!track_queue_.empty())
    {
        auto next_track = track_queue_.front();
        track_queue_.pop_front();
        beginTrackPlayback(next_track);
    }
    ui_.renderStatusBar(current_track_, playing_, player_.is_paused());
    ui_.renderTrackQueue(track_queue_);
}





