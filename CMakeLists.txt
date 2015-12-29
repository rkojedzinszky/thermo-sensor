cmake_minimum_required(VERSION 2.8)

SET(CMAKE_C_COMPILER "avr-cc")
set(CMAKE_CXX_COMPILER "avr-c++")

set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)
set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

include_directories("${CMAKE_SOURCE_DIR}")

set(MCU "attiny84a")

set(CMAKE_C_FLAGS "-O2 -mmcu=${MCU}")
set(CMAKE_CXX_FLAGS "-O2 -mmcu=${MCU} -std=c++11")

add_subdirectory(interrupt)
add_subdirectory(spi)

# add_executable(main main.cpp)
# target_link_libraries(main spi_tx5)

add_custom_target(flash
	avrdude -c usbasp -p t85 -B 5 -U flash:w:main
	DEPENDS main)

add_library(common STATIC common.cpp)

target_link_libraries(common spi)

add_executable(sensor sensor.cpp vcc_tn84a.cpp)

target_link_libraries(sensor common interrupt)

add_executable(receiver receiver.cpp)

target_link_libraries(receiver common interrupt)

add_executable(tl tl.cpp)
