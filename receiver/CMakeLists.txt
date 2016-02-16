set_mcu(atmega328p)

use_avr_crypto_lib()

add_executable(receiver receiver.cpp autoconfig.cpp)
target_link_libraries(receiver "${MCU}" ${AVR_CRYPTO_LIB_NAME})
