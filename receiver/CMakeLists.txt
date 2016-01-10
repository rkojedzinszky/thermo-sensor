set_mcu(attiny84a)

use_avr_crypto_lib()

add_executable(receiver receiver.cpp)
target_link_libraries(receiver "${MCU}" ${AVR_CRYPTO_LIB_NAME})
