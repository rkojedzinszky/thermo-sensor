set_mcu(attiny85)


add_executable(sensor sensor.cpp)
target_link_libraries(sensor "${MCU}")
