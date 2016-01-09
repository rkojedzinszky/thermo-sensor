
set(AVR_CRYPTO_LIB_SRC "${CMAKE_CURRENT_LIST_DIR}/avr-crypto-lib")

include_directories("${AVR_CRYPTO_LIB_SRC}")

set(AVR_CRYPTO_LIB_MODULES
	${AVR_CRYPTO_LIB_SRC}/aes/aes_keyschedule-asm.S
	${AVR_CRYPTO_LIB_SRC}/aes/aes_enc-asm.S
	${AVR_CRYPTO_LIB_SRC}/aes/aes_dec-asm.S
	${AVR_CRYPTO_LIB_SRC}/aes/aes_sbox-asm.S
	${AVR_CRYPTO_LIB_SRC}/aes/aes_invsbox-asm.S
)

macro(compile_avr_crypto_lib)
	add_library(${MCU}_crypto STATIC ${AVR_CRYPTO_LIB_MODULES})
endmacro(compile_avr_crypto_lib)

macro(use_avr_crypto_lib)
	include_directories("${AVR_CRYPTO_LIB_SRC}")
	set(AVR_CRYPTO_LIB_NAME ${MCU}_crypto)
endmacro(use_avr_crypto_lib)
