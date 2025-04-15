## Demo Installation Guide

Build the app:

```
mkdir build && cd build
cmake ..
cmake --build .
sudo cmake --install .
```

Then optionally install it (in order for it to be used anywhere in the terminal):

```
sudo cmake --install ./build
```

Call the app by using:

```
music-player [mp3file]
```
