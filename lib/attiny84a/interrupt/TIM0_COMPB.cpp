#include <avr/interrupt.h>
#include <interrupt/TIM0_COMPB.hpp>

ISR(TIM0_COMPB_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIM0_COMPBInterrupt::fire_)
	);
}

template <>
volatile bool TIM0_COMPBInterrupt::fire_ = false;

template <>
callback_t TIM0_COMPBInterrupt::callback_ = nullptr;
