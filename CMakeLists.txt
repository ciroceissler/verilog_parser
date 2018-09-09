cmake_minimum_required (VERSION 3.5)

project (verilog_parser)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_NAME verilog_parser)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g")

include_directories(${CMAKE_MODULE_PATH}/src)

file(GLOB SOURCES_TEST "src/*.c")

add_executable(test ${SOURCES_TEST})
