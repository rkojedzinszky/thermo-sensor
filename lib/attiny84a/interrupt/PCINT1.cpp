#include <avr/interrupt.h>
#include <interrupt/PCINT1.hpp>

ISR(PCINT1_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (PCINT1Interrupt::fire_)
	);
}

template <>
volatile bool PCINT1Interrupt::fire_ = false;

template <>
callback_t PCINT1Interrupt::callback_ = nullptr;
