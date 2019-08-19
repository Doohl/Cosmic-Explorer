# Cosmic Explorer
[![Build Status](https://travis-ci.com/Doohl/Cosmic-Explorer.svg?branch=master)](https://travis-ci.com/Doohl/Cosmic-Explorer)

**Cosmic Explorer** is a simple 2D Solar System renderer and explorer written in C++. It is powered by the [ImGUI](https://github.com/ocornut/imgui) interface library.

# Building the Code

This project uses [CMake](https://cmake.org/) for build file automation. Currently, building is only tested against Linux with the GNU C++ tools and Windows with Microsoft Visual Studio 2019.

## Linux
You need to install the SDL2 and cmake packages using your distro's package manager. For example, on Ubuntu and Debian:
```console
doohl@pc:~/Cosmic-Explorer$ apt install libsdl2-dev cmake
```
Use cmake to generate a makefile and compile. Example:
```console
doohl@pc:~/Cosmic-Explorer$ mkdir build && cd build
doohl@pc:~/Cosmic-Explorer/build$ cmake ..
...
doohl@pc:~/Cosmic-Explorer/build$ make
```

## Windows
Currently, the project is configured to work with MSVC 2019. You must have SDL2 installed on your machine. You may simply open the repository with Visual Studio 2019 and run automated cmake generation, then set the topmost `CMakeLists.txt` as build target.

# Unit Testing

Unit tests are by default compiled alongside the source code into the main binary. To run all the unit tests without running the rest of the software:
```
./bin/CosmicExplorer --exit
```