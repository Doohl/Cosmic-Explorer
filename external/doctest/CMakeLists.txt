cmake_minimum_required(VERSION 3.5)

project(doctest)

add_library(${PROJECT_NAME} doctest.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})