cmake_minimum_required(VERSION 3.5)

project(CosmicExplorer VERSION 0.0.1)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    message("=== Configuring for emscripten ===")
	add_definitions(-D__EMSCRIPTEN__)
	set(SDL2_INCLUDE_DIR "-s USE_SDL=2")
	set(SDL2_LIBRARIES "-s USE_SDL=2")
else()
	#find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
	#find_library(SDL2_LIBRARY NAME SDL2)
	find_package(SDL2 REQUIRED)
	add_subdirectory(tests)
endif()

add_subdirectory(external)
add_subdirectory(src)

add_subdirectory(data)

## 
## Build the main executable
##
add_executable(${PROJECT_NAME} WIN32 src/main.cpp)

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	target_link_libraries(${PROJECT_NAME}
		PUBLIC
			imgui
			src
	)
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		COMPILE_FLAGS "-s USE_SDL=2 -s WASM=1"
		LINK_FLAGS "-s USE_SDL=2 -s USE_WEBGL2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s BINARYEN_TRAP_MODE=clamp -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 ${FILE_FLAGS}"
	)
else()
	target_link_libraries(${PROJECT_NAME}
		PUBLIC
			imgui
			src
			${SDL2_LIBRARIES}
	)
endif()