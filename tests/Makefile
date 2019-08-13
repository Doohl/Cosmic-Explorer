SRC_DIR = ../src
TEST_DIR = ../tests
OBJ_DIR = ../obj
BIN_DIR = ../bin

EXE := $(BIN_DIR)/CosmicExplorerTest.exe

SRC_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_SOURCES := $(filter-out $(SRC_DIR)/main.cpp, $(SRC_SOURCES))
#SRC_SOURCES += ../imgui/examples/imgui_impl_sdl.cpp ../imgui/examples/imgui_impl_opengl3.cpp
#SRC_SOURCES += ../imgui/imgui.cpp imgui/imgui_demo.cpp ../imgui/imgui_draw.cpp ../imgui/imgui_widgets.cpp
SOURCE_OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SRC_SOURCES)))))

TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .test.o, $(basename $(notdir $(TEST_SOURCES)))))

SOURCES := $(TEST_SOURCES) $(SRC_SOURCES)
OBJS := $(TEST_OBJS) $(SOURCE_OBJS)
CXXFLAGS = -I../imgui/examples -I../imgui/ -I../catch2

CXXFLAGS += -g -std=c++17 -m64 -O0

# Warning levels
CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

LIBS =

## OpenGL loader
# SOURCES += ../imgui/examples/libs/gl3w/GL/gl3w.c
# CXXFLAGS += -I../imgui/examples/libs/gl3w

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
	LIBS += -static-libgcc -static-libstdc++

	CXXFLAGS += -I../imgui/examples/libs/gl3w
	CFLAGS = $(CXXFLAGS)
endif

# BUILD RULES
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(OBJ_DIR)/%.o:../imgui/examples/%.cpp
# 	@mkdir -p $(OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(OBJ_DIR)/%.o:../imgui/%.cpp
# 	@mkdir -p $(OBJ_DIR)
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(OBJ_DIR)/%.o:../imgui/examples/libs/gl3w/GL/%.c
# 	@mkdir -p $(OBJ_DIR)
# 	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.test.o:$(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
