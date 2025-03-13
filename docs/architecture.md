# Project architecture

## MP3 Playback Handling

### Decoder

The decoder class implements the minimp3 library.

Its purpose is to decode an MP3 file and return a **Track** object.

### Player

The player class handles **Track** objects.

It first needs to load a **Track** (Player::load_track(track))

Only then can the Player::play_track() method be called

- Beware this method introduces a blocking loop - it updates while the audioStream is open

- This is why it should be called in a new thread

- Methods Player::stop_track(), Player::pause_track() can be used to stop the loop from outside the object

### Track

## UI

### UiController

Uses the ncurses library

Runs the main render loop for the entire user interface
