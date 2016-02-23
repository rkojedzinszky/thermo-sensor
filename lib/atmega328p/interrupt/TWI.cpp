#include <avr/interrupt.h>
#include <interrupt/TWI.hpp>

ISR(TWI_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TWIInterrupt::fire_)
	);
}

template <>
volatile bool TWIInterrupt::fire_ = false;
