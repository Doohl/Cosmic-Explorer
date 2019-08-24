cmake_minimum_required(VERSION 3.5)

project(src)

add_library(${PROJECT_NAME}
	entity.cpp
	kepler_orbit.cpp
	logic_manager.cpp
	ui_manager.cpp
	vec2.cpp
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		imgui
	PUBLIC
		nlohmann_json
)

if(MSVC OR MSYS OR MINGW)
	message("Configuring for Windows (MSVC, MSYS, MINGW)")
	target_link_libraries(${PROJECT_NAME}
		PUBLIC
			opengl32.lib
	)
else()
	message("Configuring for Linux")
	target_link_libraries(${PROJECT_NAME}
		PUBLIC
			-static-libgcc
			-static-libstdc++
			-lGL
			-ldl
	)
endif()

# Set compiler warnings here
target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
	>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4
	>
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${SDL2_INCLUDE_DIR}
)
