# The documentation of Music Player

Welcome to the full technical documentation of music-player - a simple and lightweight UNIX terminal mp3 player app.

Created for the course **NPRG041: Programming in C++** at Charles University.

---

## Features

- **File explorer**
  - Navigate directories in *midnight commander* style
- **MP3 file playback**
  - Volume control
  - Pause/Resume
- **Track queue**
  - Automatic fetching from queue upon track completion
  - Skip/Add controls

### Controls

- `q` — Quit
- `a` — Add track to queue
- `n` — Skip to next queued track
- `Space` — Pause/Resume
- `u/d` — Volume up/down
- `Enter` — Play selected track

---

## Architecture 

The program is divided into multiple components:
- `Decoder` — Loads and processes MP3 files
- `Player` — Manages playback in a background thread
- `UiController` — Handles user input and control flow
- `UiRenderer` — Encapsulates rendering logic using ncurses

Visit [architecture.md](architecture.md) for more details.

## Dependencies 

- `C++ 20`
- [`RtAudio`](https://github.com/thestk/rtaudio) — for real-time audio output
- [`minimp3`](https://github.com/lieff/minimp3) — for MP3 decoding
- [`ncurses`](https://github.com/mirror/ncurses) — for terminal UI

## Other requirements

- Only supported on `Linux` or `macOS`
- Requires an audio backend installed and properly set-up
  - In case of Linux: either `ALSA`, `PulseAudio`, `JACK` or `OSS`
  - On macOS: `CoreAudio` or `JACK`

## Example/Usage walkthrough

In case you need further instructions and an example of the app's use scenario visit [example.md](example.md).
