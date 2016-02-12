#include <avr/interrupt.h>
#include <interrupt/TIMER1_COMPB.hpp>

ISR(TIMER1_COMPB_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIMER1_COMPBInterrupt::fire_)
	);
}

template <>
volatile bool TIMER1_COMPBInterrupt::fire_ = false;
