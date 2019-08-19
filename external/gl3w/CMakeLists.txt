cmake_minimum_required(VERSION 3.5)

add_library(gl3w gl3w.c)

target_include_directories(gl3w PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})