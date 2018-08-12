cmake_minimum_required (VERSION 3.5)

project (verilog_parser)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_NAME verilog_parser)
set(LIB_NAME _verilog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g")

include_directories(${CMAKE_MODULE_PATH}/src)

add_library(${LIB_NAME} STATIC src/verilog_parser.cpp)
set_property(TARGET ${LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

file(GLOB SOURCES_DEBUG "src/*.cpp")

add_executable(debug ${SOURCES_DEBUG})

file(GLOB SOURCES_TEST "src/*.c")

add_executable(test ${SOURCES_TEST})
target_link_libraries(test ${LIB_NAME} stdc++)
