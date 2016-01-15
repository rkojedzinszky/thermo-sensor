#include <avr/interrupt.h>
#include <interrupt/TIMER0_OVF.hpp>

ISR(TIMER0_OVF_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIMER0_OVFInterrupt::fire_)
	);
}

template <>
volatile bool TIMER0_OVFInterrupt::fire_ = false;

template <>
callback_t TIMER0_OVFInterrupt::callback_ = nullptr;
