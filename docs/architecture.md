# Architecture overview

## MP3 Playback Handling

### Decoder

The decoder class implements the minimp3 library.

Its purpose is to decode an MP3 file and return a **Track** object.

### Player

The player class handles **Track** objects.

It first needs to load a **Track** (Player::load_track(track))

Its main method - Player::play_track() is intended to be used on a separate thread.

- The method can be then controlled from the outside using the public interface e.g. Player::pause_track()

### Track

A set of classes that hold track info 

- General track information - metadata
- Data crucial for playback - sample rate, PCM data, channel count

## UI

The UI of the app is created with the support of ncurses

### UiController

Runs the main render loop for the entire user interface

Takes care of user input

Evaluates current states and delegates rendering jobs to the renderer

### UiRenderer

Helper class for the UiController

Receives data from the controller and updates specific windows of the app
