cmake_minimum_required(VERSION 3.5)

add_executable(CosmicExplorer
	main.cpp
	entity.cpp
	kepler_orbit.cpp
	logic_manager.cpp
	vec2.cpp
)

target_link_libraries(CosmicExplorer
	PRIVATE
		gl3w
		imgui
		doctest
		nlohmann_json
)

if(MSVC)
	target_link_libraries(CosmicExplorer
		PRIVATE
			gl3w
			imgui
			doctest
			nlohmann_json
		PUBLIC
			${SDL2_LIBRARIES}
			opengl32.lib
	)
elseif(MSYS OR MINGW)
	target_link_libraries(CosmicExplorer
		PRIVATE
			gl3w
			imgui
			doctest
			nlohmann_json
		PUBLIC
			${SDL2_LIBRARIES}
			opengl32.lib
			-static-libgcc
			-static-libstdc++
			-lgdi32
			-limm32
			-mconsole
			-mwindows
	)
	target_compile_options(CosmicExplorer PRIVATE -g -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
else()
	target_link_libraries(CosmicExplorer
		PRIVATE
			gl3w
			imgui
			doctest
			nlohmann_json
		PUBLIC
			-static-libgcc
			-static-libstdc++
			-lGL
			-ldl
			${SDL2_LIBRARIES}
	)
	target_compile_options(CosmicExplorer PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif()
target_include_directories(CosmicExplorer PUBLIC ${SDL2_INCLUDE_DIR})