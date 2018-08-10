cmake_minimum_required (VERSION 3.5)

project (verilog_parser)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_NAME verilog_parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -g")

include_directories(${CMAKE_MODULE_PATH}/src)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

