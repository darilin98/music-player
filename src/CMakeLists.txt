cmake_minimum_required(VERSION 3.29)
project(music-player)

set(CMAKE_CXX_STANDARD 20)

# Find RtAudio
find_library(RTAUDIO_LIB NAMES rtaudio PATHS /opt/homebrew/lib)
include_directories(/opt/homebrew/include)

# Add executable
add_executable(music-player main.cpp
        player.cpp
        minimp3/minimp3.c)

# Link RtAudio
target_link_libraries(music-player ${RTAUDIO_LIB} pthread)