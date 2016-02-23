#include <avr/interrupt.h>
#include <interrupt/SPM_READY.hpp>

ISR(SPM_READY_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (SPM_READYInterrupt::fire_)
	);
}

template <>
volatile bool SPM_READYInterrupt::fire_ = false;
