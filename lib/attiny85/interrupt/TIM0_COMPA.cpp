#include <avr/interrupt.h>
#include <interrupt/TIM0_COMPA.hpp>

ISR(TIM0_COMPA_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIM0_COMPAInterrupt::fire_)
	);
}

template <>
volatile bool TIM0_COMPAInterrupt::fire_ = false;
