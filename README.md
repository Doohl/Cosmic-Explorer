# Cosmic Explorer
[![Build Status](https://travis-ci.com/Doohl/Cosmic-Explorer.svg?branch=master)](https://travis-ci.com/Doohl/Cosmic-Explorer)

**Cosmic Explorer** is a simple 2D Solar System renderer and explorer written in C++. It is powered by the [ImGUI](https://github.com/ocornut/imgui) interface library.

![UI example](https://i.imgur.com/HXFtEoy.png)

# Building the Code

This project uses [CMake](https://cmake.org/) for build file generation. Building is currently only tested against Linux and Windows.

## Linux
You need to install the SDL2 and cmake packages using your distro's package manager. For example, on Ubuntu and Debian:
```console
$ apt install libsdl2-dev cmake
```
Use cmake to generate a makefile and compile. Example:
```console
$ mkdir build && cd build
$ cmake ..
```
```console
$ make
```
OR:
```console
$ cmake --build .
```

## Windows

### Visual Studio 2019
You must have SDL2 installed in Visual Studio's default lib, include, bin directories (or anywhere that the `cmake/FindSDL2.cmake` script is able to find it in). You may simply open the repository with Visual Studio 2019 and run automated cmake generation, then set the root `CMakeLists.txt` as build target.

Optionally, you may use the "Developer Command Prompt for VS 2019" shortcut to generate and compile in the command-line:
```console
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe ..
```

Building:
```console
ninja
```

The pre-packaged VS Code settings demonstrate a working commandline configure-and-build setup. 

### MSYS / MinGW32
Instructions similar to Linux, with the exception that you will want to override the cmake default build system:
```console
$ mkdir build && cd build
$ cmake -G "MSYS Makefiles" ..
```

You will need to install SDL2 64-bit. Example on MSYS:
```console
$ pacman -S mingw-w64-x86_64-SDL2
```

```console
$ mingw32-make
```

## Emscripten (WASM)
This project supports a WASM compile target. You must have the [Emscripten](https://emscripten.org/) suite installed and configured to build to WASM.
```console
emconfigure cmake ..
emmake make
emmake make install
```

# Unit Testing
Unit tests use [doctest](https://github.com/onqtam/doctest). To run, you may do:
```console
$ cd bin
$ ./tests/vec2
$ ./tests/entity
...
```
OR, through CTests:
```console
$ make test ARGS=-j4
```
```console
$ ctest . j4
```
