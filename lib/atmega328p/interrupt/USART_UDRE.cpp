#include <avr/interrupt.h>
#include <interrupt/USART_UDRE.hpp>

ISR(USART_UDRE_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (USART_UDREInterrupt::fire_)
	);
}

template <>
volatile bool USART_UDREInterrupt::fire_ = false;
