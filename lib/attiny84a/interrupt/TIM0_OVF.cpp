#include <avr/interrupt.h>
#include <interrupt/TIM0_OVF.hpp>

ISR(TIM0_OVF_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIM0_OVFInterrupt::fire_)
	);
}

template <>
volatile bool TIM0_OVFInterrupt::fire_ = false;
