#include <avr/interrupt.h>
#include <interrupt/EE_READY.hpp>

ISR(EE_READY_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (EE_READYInterrupt::fire_)
	);
}

template <>
volatile bool EE_READYInterrupt::fire_ = false;
