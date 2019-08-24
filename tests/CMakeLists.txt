cmake_minimum_required(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)

function(CREATE_TEST TEST_NAME)
	add_executable(${TEST_NAME} main.cpp ${TEST_NAME}.cpp)
	target_link_libraries(${TEST_NAME} PUBLIC doctest src)
	add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

create_test(entity)
create_test(kepler_orbit)
create_test(logic_manager)
create_test(vec2)
