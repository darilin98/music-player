//
// Created by Darek Rudiš on 25.02.2025.
//
#include "ui_controller.hpp"

#include <queue>
constexpr char KEY_QUIT = 'q';
constexpr char KEY_PAUSE = ' ';
constexpr char KEY_NEXT_QUEUE = 'n';
constexpr char KEY_ADD_QUEUE = 'a';
constexpr char KEY_PLAY_TRACK = '\n';

using queue_t = std::queue<track_ptr_t>;

UiController::UiController()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(0);
}

void UiController::beginRenderLoop()
{
    std::vector<std::string> files;
    queue_t track_queue;
    std::string path = std::filesystem::current_path().string();
    int highlight = 0;
    std::thread playback_thread;
    bool playing = false;
    bool running = true;

    updateFileList(files, path);

    while (running)
    {
        renderFileList(files, highlight);
        switch (int pressed_key = getch()) {
            case KEY_UP:
                if (highlight > 0) highlight--;
                break;
            case KEY_DOWN:
                if (highlight < files.size() - 1) highlight++;
                break;
            case KEY_PLAY_TRACK:
                processTrackSelection(files, highlight, path, playback_thread, playing);
                break;
            case KEY_PAUSE:
                if (playing) {
                    player.is_paused() ? player.resume_track() : player.pause_track();
                }
                break;
            case KEY_QUIT:
                stopTrackPlayback(playback_thread, playing);
                running = false;
                break;
        }
    }
    endwin();
}

void UiController::beginTrackPlayback(std::thread& playback_thread, const int& highlight, std::vector<std::string>& files, bool& playing, const std::string& path)
{
    name_t track_name = path + "/" + files[highlight];
    track_ptr_t track = dec.decode_mp3(track_name);
    if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
    {
        throw std::runtime_error("Error decoding track!");
    }
    player.load_track(track);

    playing = true;
    playback_thread = std::thread([this, &playing]() {
        player.play_track();
        playing = false;
    });
}
void UiController::stopTrackPlayback(std::thread &playback_thread, bool &playing)
{
    if (playing)
    {
        player.stop_track();
        if (playback_thread.joinable())
            playback_thread.join();
        playing = false;
    }
}

void UiController::updateFileList(std::vector<std::string> &files, const std::string &path)
{
    files.clear();
    files.emplace_back(".."); // For going up a directory
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        files.push_back(entry.path().filename().string());
    }
}

void UiController::renderFileList(const std::vector<std::string> &files, const int &highlight)
{
    clear();
    for (size_t i = 0; i < files.size(); ++i) {
        if (i == highlight) {
            attron(A_REVERSE); // Highlight selected file
        }
        mvprintw(i, 0, files[i].c_str());
        if (i == highlight) {
            attroff(A_REVERSE);
        }
    }
    refresh();
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

void UiController::processTrackSelection(std::vector<std::string>& files, int& highlight, name_t& path, std::thread& playback_thread, bool& playing)
{
    name_t selected = files[highlight];
    name_t new_path = (selected == "..") ? std::filesystem::path(path).parent_path().string() : path + "/" + selected;
    if (std::filesystem::is_directory(new_path))
    {
        path = new_path;
        highlight = 0;
        updateFileList(files, path);
    } else
    {
        if (playing)
            stopTrackPlayback(playback_thread, playing);
        try
        {
            beginTrackPlayback(playback_thread, highlight, files,  playing, path);
        } catch (const std::exception& e)
        {
            showErrorPopup("Error opening file!");
        }
    }
}





