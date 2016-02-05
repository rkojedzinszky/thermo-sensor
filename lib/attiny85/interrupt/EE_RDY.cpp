#include <avr/interrupt.h>
#include <interrupt/EE_RDY.hpp>

ISR(EE_RDY_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (EE_RDYInterrupt::fire_)
	);
}

template <>
volatile bool EE_RDYInterrupt::fire_ = false;
