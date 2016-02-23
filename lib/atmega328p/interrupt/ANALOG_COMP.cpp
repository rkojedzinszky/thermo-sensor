#include <avr/interrupt.h>
#include <interrupt/ANALOG_COMP.hpp>

ISR(ANALOG_COMP_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (ANALOG_COMPInterrupt::fire_)
	);
}

template <>
volatile bool ANALOG_COMPInterrupt::fire_ = false;
