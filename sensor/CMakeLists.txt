set(MCU "attiny85")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=${MCU}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmcu=${MCU}")

include_directories("${CMAKE_SOURCE_DIR}/lib/${MCU}")

add_executable(sensor sensor.cpp)
target_link_libraries(sensor "${MCU}")
