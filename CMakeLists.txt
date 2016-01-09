cmake_minimum_required(VERSION 2.8)

set(CMAKE_ASM_COMPILER "avr-gcc")
SET(CMAKE_C_COMPILER "avr-gcc")
set(CMAKE_CXX_COMPILER "avr-c++")

enable_language(ASM)

set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)
set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

include_directories("${CMAKE_SOURCE_DIR}/common/include")

set(CMAKE_ASM_FLAGS "-O2")
set(CMAKE_C_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "-O2 -std=c++11")

macro(set_mcu mcu)
set(MCU "${mcu}")
include_directories("${CMAKE_SOURCE_DIR}/lib/${MCU}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mmcu=${MCU}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=${MCU}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmcu=${MCU}")
endmacro(set_mcu)

add_subdirectory(lib)
add_subdirectory(sensor)
add_subdirectory(receiver)
