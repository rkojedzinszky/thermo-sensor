#include <avr/interrupt.h>
#include <interrupt/USI_START.hpp>

ISR(USI_START_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (USI_STARTInterrupt::fire_)
	);
}

template <>
volatile bool USI_STARTInterrupt::fire_ = false;
