#include <avr/interrupt.h>
#include <interrupt/TIM1_COMPA.hpp>

ISR(TIM1_COMPA_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIM1_COMPAInterrupt::fire_)
	);
}

template <>
volatile bool TIM1_COMPAInterrupt::fire_ = false;

template <>
callback_t TIM1_COMPAInterrupt::callback_ = nullptr;
