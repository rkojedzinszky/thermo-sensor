#include <avr/interrupt.h>
#include <interrupt/USI_OVF.hpp>

ISR(USI_OVF_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (USI_OVFInterrupt::fire_)
	);
}

template <>
volatile bool USI_OVFInterrupt::fire_ = false;
