project(CAdvanced C)
cmake_minimum_required(VERSION 2.8)

add_subdirectory(libfdr)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/libfdr)

add_subdirectory(code)