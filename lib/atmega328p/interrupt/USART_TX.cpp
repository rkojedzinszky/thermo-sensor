#include <avr/interrupt.h>
#include <interrupt/USART_TX.hpp>

ISR(USART_TX_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (USART_TXInterrupt::fire_)
	);
}

template <>
volatile bool USART_TXInterrupt::fire_ = false;
