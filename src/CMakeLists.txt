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
			SDL2.lib
			SDL2main.lib
			opengl32.lib
	)
else()
	target_link_libraries(CosmicExplorer
		PRIVATE
			gl3w
			imgui
			doctest
			nlohmann_json
		PUBLIC
			`sdl2-config --libs`
			-lGL
			-ldl
	)
endif()
target_include_directories(CosmicExplorer PUBLIC ${SDL2_INCLUDE_DIR})