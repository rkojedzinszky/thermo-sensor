#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<USART_TX_vect_num> USART_TXInterrupt;
