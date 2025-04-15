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

target_link_libraries(music-player PRIVATE rtaudio pthread ncurses)

set_target_properties(music-player PROPERTIES
        INSTALL_RPATH "/usr/local/lib"
)

install(TARGETS music-player DESTINATION bin)
