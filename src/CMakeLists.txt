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
else()
	target_link_libraries(CosmicExplorer
		PRIVATE
			gl3w
			imgui
			doctest
			nlohmann_json
		PUBLIC
			-lGL
			-ldl
			${SDL2_LIBRARIES}
	)
endif()
target_include_directories(CosmicExplorer PUBLIC ${SDL2_INCLUDE_DIR})