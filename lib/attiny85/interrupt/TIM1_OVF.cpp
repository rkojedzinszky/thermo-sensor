#include <avr/interrupt.h>
#include <interrupt/TIM1_OVF.hpp>

ISR(TIM1_OVF_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIM1_OVFInterrupt::fire_)
	);
}

template <>
volatile bool TIM1_OVFInterrupt::fire_ = false;

template <>
callback_t TIM1_OVFInterrupt::callback_ = nullptr;
