set_mcu(attiny85)

use_avr_crypto_lib()

add_executable(sensor sensor.cpp)
target_link_libraries(sensor "${MCU}" ${AVR_CRYPTO_LIB_NAME})
