set_mcu(atmega328p)

add_subdirectory(interrupt)

add_library("${MCU}"
	../common.cpp
	../config.cpp
)
target_link_libraries("${MCU}" "${MCU}_interrupt")

compile_avr_crypto_lib()
