set_mcu(attiny84a)

add_executable(receiver receiver.cpp)
target_link_libraries(receiver "${MCU}")
