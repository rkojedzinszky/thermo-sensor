#include <avr/interrupt.h>
#include <interrupt/PCINT0.hpp>

ISR(PCINT0_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (PCINT0Interrupt::fire_)
	);
}

template <>
volatile bool PCINT0Interrupt::fire_ = false;

template <>
callback_t PCINT0Interrupt::callback_ = nullptr;
