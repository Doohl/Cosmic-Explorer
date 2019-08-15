# Before running make:
#	git submodule init
#	git submodule update
#
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-x86_64-SDL2

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

EXE := $(BIN_DIR)/CosmicExplorer

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += imgui/examples/imgui_impl_sdl.cpp imgui/examples/imgui_impl_opengl3.cpp
SOURCES += imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS += -g -std=c++17 -m64 -MMD -O0

# Warning levels
CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

# ImGUI resources
INCLUDES = -Iimgui/examples -Iimgui/

## OpenGL loader
SOURCES += imgui/examples/libs/gl3w/GL/gl3w.c
INCLUDES += -Iimgui/examples/libs/gl3w -Iinclude

UNAME_S := $(shell uname -s)

# LINUX FLAGS
ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"
	LIBS = -lGL -ldl `sdl2-config --libs`

	INCLUDES += `sdl2-config --cflags`
	CFLAGS = -static-libgcc -Iimgui/examples/libs/gl3w
endif

# OS X FLAGS (NOT TESTED)
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
	LIBS = -static-libgcc -static-libstdc++ -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

	CXXFLAGS += -mconsole -mwindows
	INCLUDES += `pkg-config --cflags sdl2`
	CFLAGS = -static-libgcc -Iimgui/examples/libs/gl3w
endif

CXXFLAGS := $(CXXFLAGS) $(INCLUDES)

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
	rm -rf $(BIN_DIR)/data
	cp -R data $(BIN_DIR)/data/

$(EXE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
