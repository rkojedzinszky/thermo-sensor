set_mcu(attiny85)

use_avr_crypto_lib()

add_executable(sensor main.cpp
	sensor.cpp
	sensor_init.cpp
	sensor_send.cpp
	sensor_loop.cpp
	autoconfig.cpp
	)
target_link_libraries(sensor "${MCU}" ${AVR_CRYPTO_LIB_NAME})
