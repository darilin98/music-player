cmake_minimum_required(VERSION 3.15)
project(music-player)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(FETCHCONTENT_SOURCE_DIR_RTAUDIO "" CACHE STRING "")
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(
        RtAudio
        GIT_REPOSITORY https://github.com/thestk/rtaudio.git
        GIT_TAG 6.0.1
)
FetchContent_MakeAvailable(RtAudio)

FetchContent_Declare(
    minimp3
    GIT_REPOSITORY https://github.com/lieff/minimp3.git
    GIT_TAG master
)
FetchContent_MakeAvailable(minimp3)

FetchContent_Declare(
    ncurses
    GIT_REPOSITORY https://github.com/mirror/ncurses.git
    GIT_TAG master
)
FetchContent_MakeAvailable(ncurses)

add_subdirectory(src)

include(CPack)

