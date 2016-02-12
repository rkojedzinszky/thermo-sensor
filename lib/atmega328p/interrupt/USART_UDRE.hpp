#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<USART_UDRE_vect_num> USART_UDREInterrupt;
