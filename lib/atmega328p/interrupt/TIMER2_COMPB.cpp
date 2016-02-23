#include <avr/interrupt.h>
#include <interrupt/TIMER2_COMPB.hpp>

ISR(TIMER2_COMPB_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIMER2_COMPBInterrupt::fire_)
	);
}

template <>
volatile bool TIMER2_COMPBInterrupt::fire_ = false;
