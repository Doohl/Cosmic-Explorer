cmake_minimum_required(VERSION 3.5)

project(imgui)

add_library(${PROJECT_NAME}
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_widgets.cpp
	imgui_impl_sdl.cpp
	imgui_impl_opengl3.cpp
)
target_include_directories(${PROJECT_NAME}
	PRIVATE 
		${SDL2_INCLUDE_DIR}
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR} 
)
target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		${SDL2_LIBRARIES}
	PUBLIC
		gl3w
)