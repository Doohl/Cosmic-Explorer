cmake_minimum_required(VERSION 3.5)

if(NOT (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten"))
	add_subdirectory(doctest)
	add_subdirectory(gl3w)
endif()

add_subdirectory(imgui)
add_subdirectory(nlohmann_json)