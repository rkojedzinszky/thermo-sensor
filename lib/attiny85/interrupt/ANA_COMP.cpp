#include <avr/interrupt.h>
#include <interrupt/ANA_COMP.hpp>

ISR(ANA_COMP_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (ANA_COMPInterrupt::fire_)
	);
}

template <>
volatile bool ANA_COMPInterrupt::fire_ = false;
