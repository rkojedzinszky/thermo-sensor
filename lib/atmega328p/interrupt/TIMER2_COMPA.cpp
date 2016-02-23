#include <avr/interrupt.h>
#include <interrupt/TIMER2_COMPA.hpp>

ISR(TIMER2_COMPA_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIMER2_COMPAInterrupt::fire_)
	);
}

template <>
volatile bool TIMER2_COMPAInterrupt::fire_ = false;
