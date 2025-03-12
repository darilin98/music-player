//
// Created by Darek Rudiš on 25.02.2025.
//
#include "ui_controller.hpp"
constexpr char KEY_QUIT = 'q';

UiController::UiController()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
}

void UiController::beginRenderLoop()
{

    std::vector<std::string> files;
    std::string path = std::filesystem::current_path().string();
    int highlight = 0;
    std::thread playback_thread;
    bool playing = false;

    updateFileList(files, path);

    while (true)
    {
        renderFileList(files, highlight);
        int pressed_key = getch();
        if (pressed_key == KEY_UP && highlight > 0) {
            highlight--;
        } else if (pressed_key == KEY_DOWN && highlight < files.size() - 1) {
            highlight++;
        } else if (pressed_key == '\n') {
            std::string selected = files[highlight];
            std::string new_path = (selected == "..") ? std::filesystem::path(path).parent_path().string() : path + "/" + selected;
            if (std::filesystem::is_directory(new_path))
            {
                path = new_path;
                highlight = 0;
                updateFileList(files, path);
            } else
            {
                if (playing)
                    stopTrackPlayback(playback_thread, playing);
                beginTrackPlayback(playback_thread, highlight, files,  playing);
            }
        } else if (pressed_key == ' ')
        {
            if (playing) {
                if (player.is_paused()) {
                    player.resume_track();
                } else {
                    player.pause_track();
                }
            }
        }
        else if (pressed_key == KEY_QUIT) {
            stopTrackPlayback(playback_thread, playing);
            break;
        }
    }
    endwin();
}

void UiController::beginTrackPlayback(std::thread& playback_thread, const int& highlight, std::vector<std::string>& files, bool& playing)
{
    name_t& track_name = files[highlight];
    track_ptr_t track = dec.decode_mp3(track_name);
    if (dynamic_cast<ErrorTrack*>(track.get()) != nullptr) //Check type of returned track
    {
        std::cerr << "Error decoding audio file!" << std::endl;
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




