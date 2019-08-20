cmake_minimum_required(VERSION 3.5)

project(tests)

add_executable(${PROJECT_NAME}
	main.cpp
	entity.cpp
	kepler_orbit.cpp
	logic_manager.cpp
	vec2.cpp
)


target_link_libraries(${PROJECT_NAME}
	PUBLIC
		doctest
		src
)