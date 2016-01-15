#include <avr/interrupt.h>
#include <interrupt/TIM1_COMPB.hpp>

ISR(TIM1_COMPB_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIM1_COMPBInterrupt::fire_)
	);
}

template <>
volatile bool TIM1_COMPBInterrupt::fire_ = false;

template <>
callback_t TIM1_COMPBInterrupt::callback_ = nullptr;
