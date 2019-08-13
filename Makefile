# Requires SDL2
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2 (https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2):
#   pacman -S mingw-w64-i686-SDL2

all:
	mingw32-make -C ./src
clean:
	mingw32-make -C ./src clean
test:
	mingw32-make -C ./tests
	./bin/CosmicExplorerTest