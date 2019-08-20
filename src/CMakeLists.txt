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

if(MSVC)
	target_link_libraries(${PROJECT_NAME}
		PUBLIC
			opengl32.lib
	)
elseif(MSYS OR MINGW)
	target_link_libraries(${PROJECT_NAME}
		PUBLIC
			opengl32.lib
			-static-libgcc
			-static-libstdc++
			-lgdi32
			-limm32
			-mconsole
			-mwindows
			-lmingw32
	)
	target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
else()
	target_link_libraries(${PROJECT_NAME}
		PUBLIC
			-static-libgcc
			-static-libstdc++
			-lGL
			-ldl
	)
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${SDL2_INCLUDE_DIR}
)
