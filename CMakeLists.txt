cmake_minimum_required(VERSION 2.8)

SET(CMAKE_C_COMPILER "avr-cc")
set(CMAKE_CXX_COMPILER "avr-c++")

set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)
set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

include_directories("${CMAKE_SOURCE_DIR}/common/include")

set(CMAKE_C_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "-O2 -std=c++11")

add_subdirectory(lib)
add_subdirectory(sensor)
add_subdirectory(receiver)
