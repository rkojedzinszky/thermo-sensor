#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<USART_RX_vect_num> USART_RXInterrupt;
