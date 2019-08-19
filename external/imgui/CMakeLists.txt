cmake_minimum_required(VERSION 3.5)

add_library(imgui
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_widgets.cpp
	imgui_impl_sdl.cpp
	imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIR})
target_link_libraries(imgui PRIVATE gl3w)