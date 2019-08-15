# Cosmic Explorer
[![Build Status](https://travis-ci.com/Doohl/Cosmic-Explorer.svg?branch=master)](https://travis-ci.com/Doohl/Cosmic-Explorer)

**Cosmic Explorer** is a simple 2D Solar System renderer and explorer written in C++. It is powered by the [ImGUI](https://github.com/ocornut/imgui) interface library.

# Building the Code

The project uses a git submodule for its ImGUI dependency, which needs to be initialized with the command:
```
git submodule update --init --recursive
```

Until a proper CMake build system is implemented, building can only be done on Windows with MSYS2 or Linux.

## Linux
You need to install the SDL2 package using your distro's package manager. For example, on Ubuntu and Debian:
```
apt-get install libsdl2-dev
```
Then:
```
make
```

## Windows
Currently, the project is only configured to work with MSYS2, with the MinGW64 compiler. To install SDL2:
```
pacman -S mingw-w64-x86_64-SDL2
```
Then:
```
mingw32-make
```

# Unit Testing

Unit tests are by default compiled alongside the source code into the main binary. To run all the unit tests without running the rest of the software:
```
./bin/CosmicExplorer --exit
```

