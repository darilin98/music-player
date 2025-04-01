cmake_minimum_required(VERSION 3.15)
project(music-player)

set(CMAKE_CXX_STANDARD 20)

# Enable FetchContent for dependencies
include(FetchContent)

# Fetch RtAudio
FetchContent_Declare(
        RtAudio
        GIT_REPOSITORY https://github.com/thestk/rtaudio.git
        GIT_TAG master
)
FetchContent_MakeAvailable(RtAudio)

# Fetch minimp3
FetchContent_Declare(
        minimp3
        GIT_REPOSITORY https://github.com/lieff/minimp3.git
        GIT_TAG master
)
FetchContent_MakeAvailable(minimp3)

# Fetch ncurses
FetchContent_Declare(
        ncurses
        GIT_REPOSITORY https://github.com/mirror/ncurses.git
        GIT_TAG master
)
FetchContent_MakeAvailable(ncurses)

# Add executable
add_executable(music-player
        main.cpp
        player.cpp
        minimp3/minimp3.c
        player.hpp
        decoder.hpp
        decoder.cpp
        ui_controller.hpp
        ui_controller.cpp
        ui_renderer.hpp
        ui_renderer.cpp
)

set_target_properties(music-player PROPERTIES
    INSTALL_RPATH "/usr/local/lib"
)

# Link RtAudio
target_link_libraries(music-player PRIVATE rtaudio pthread ncurses)

# INSTALLATION RULES
install(TARGETS music-player DESTINATION bin)  # Install the binary to /usr/local/bin or equivalent

# Packaging Support
include(CPack)
