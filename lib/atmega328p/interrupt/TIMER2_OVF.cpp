#include <avr/interrupt.h>
#include <interrupt/TIMER2_OVF.hpp>

ISR(TIMER2_OVF_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIMER2_OVFInterrupt::fire_)
	);
}

template <>
volatile bool TIMER2_OVFInterrupt::fire_ = false;
