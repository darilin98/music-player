# Music Player

A simple and lightweight UNIX terminal mp3 player app

Created for the course **NPRG041: Programming in C++** at Charles University

## Quick Start

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

Call the app in your terminal by using:

```
music-player 
```

## Trying it out

If you don't have mp3 files on hand you can go to /src/testmp3files/

This directory contains some royalty-free tracks to test the app's behavior 

## Further Reading

[Project specification](docs/project_specification.md).

[Full documentation pdf](docs/doxygen/latex/refman.pdf)

GitLab pages aren't enabled on this repo but the HTML documentation output can be generated using the CI job.
