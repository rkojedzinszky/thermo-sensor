#include <avr/interrupt.h>
#include <interrupt/TIM1_CAPT.hpp>

ISR(TIM1_CAPT_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIM1_CAPTInterrupt::fire_)
	);
}

template <>
volatile bool TIM1_CAPTInterrupt::fire_ = false;

template <>
callback_t TIM1_CAPTInterrupt::callback_ = nullptr;
