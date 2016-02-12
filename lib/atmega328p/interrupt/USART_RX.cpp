#include <avr/interrupt.h>
#include <interrupt/USART_RX.hpp>

ISR(USART_RX_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (USART_RXInterrupt::fire_)
	);
}

template <>
volatile bool USART_RXInterrupt::fire_ = false;
