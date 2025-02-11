# UNIX Terminal Music Player

## Goals:

Implement a music player capable of playing MP3 files operating in the UNIX command line interface.

Allow the user to have control over the music - pause, play, skip, add to queue, volume control.

Make the app compatible with Linux and macOS systems.

## Functionalities:

1. MP3 Playback

   - Load and decode an MP3 file
   - Send PCM data to audio hardware API

2. Text-based user interface (TUI)

   - Intuitive and easy-to-use controls for file playing
   - Design inspired by GNU Midnight Commander

3. Display current track information

   - Metadata from the loaded file

4. Player controls

   - Volume control
   - Pause and play
   - Skip

5. Track queue
   - Skip feature grabs tracks from the queue

## Libraries:

1. MP3 File Handling

   - MP3 files are encoded in a very complex way. This means I need to use an external library to decode them.
   - **minimp3**:
     - minimp3 is a very minimalistic single-header library written in C

2. Audio hardware API

   - For the communication with hardware I will be using **rtaudio**
   - It is a set of C++ classes that will allow me to open a stream for the PCM data on both Linux and macOS

3. User interface
   - To achieve the look and feel of Midnight Commander I chose to use **ncurses**
