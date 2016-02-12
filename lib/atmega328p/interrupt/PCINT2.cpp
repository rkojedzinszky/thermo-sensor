#include <avr/interrupt.h>
#include <interrupt/PCINT2.hpp>

ISR(PCINT2_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (PCINT2Interrupt::fire_)
	);
}

template <>
volatile bool PCINT2Interrupt::fire_ = false;
