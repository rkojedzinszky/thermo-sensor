#include <avr/interrupt.h>
#include <interrupt/USI_STR.hpp>

ISR(USI_STR_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (USI_STRInterrupt::fire_)
	);
}

template <>
volatile bool USI_STRInterrupt::fire_ = false;
