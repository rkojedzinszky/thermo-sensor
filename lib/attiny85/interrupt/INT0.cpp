#include <avr/interrupt.h>
#include <interrupt/INT0.hpp>

ISR(INT0_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (INT0Interrupt::fire_)
	);
}

template <>
volatile bool INT0Interrupt::fire_ = false;
