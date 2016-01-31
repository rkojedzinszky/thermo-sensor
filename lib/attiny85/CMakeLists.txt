set_mcu(attiny85)

add_subdirectory(interrupt)

add_library("${MCU}"
	usi.cpp
	vcc.cpp
	../common.cpp
	../config.cpp
)
target_link_libraries("${MCU}" "${MCU}_interrupt")

compile_avr_crypto_lib()
