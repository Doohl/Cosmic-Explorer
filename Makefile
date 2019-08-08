# Requires SDL2
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2 (https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2):
#   pacman -S mingw-w64-i686-SDL2

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

EXE := $(BIN_DIR)/CosmicExplorer

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += imgui/examples/imgui_impl_sdl.cpp imgui/examples/imgui_impl_opengl3.cpp
SOURCES += imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS = -Iimgui/examples -Iimgui/

CXXFLAGS += -g -std=c++17 -m64

# Warning levels
CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

LIBS =

## OpenGL loader
SOURCES += imgui/examples/libs/gl3w/GL/gl3w.c
CXXFLAGS += -Iimgui/examples/libs/gl3w

UNAME_S := $(shell uname -s)

# LINUX FLAGS
ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`

	CXXFLAGS += -I../libs/gl3w `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

# OS X FLAGS
ifeq ($(UNAME_S), Darwin)
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += -I../libs/gl3w `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

# WINDOWS FLAGS
ifeq ($(OS),Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -static-libgcc -static-libstdc++ -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

	CXXFLAGS += -Iimgui/examples/libs/gl3w `pkg-config --cflags sdl2`
	CFLAGS = $(CXXFLAGS)
endif

# BUILD RULES
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:imgui/examples/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:imgui/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:imgui/examples/libs/gl3w/GL/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
