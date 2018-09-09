cmake_minimum_required (VERSION 3.5)

project (verilog_parser)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_NAME verilog_parser)
set(LIB_NAME _create_verilog)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g")

include_directories(${CMAKE_MODULE_PATH}/src)

add_library(${LIB_NAME} STATIC src/create_verilog.c)
set_property(TARGET ${LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

file(GLOB SOURCES_TEST "src/main.c")

add_executable(main ${SOURCES_TEST})
target_link_libraries(main ${LIB_NAME})
