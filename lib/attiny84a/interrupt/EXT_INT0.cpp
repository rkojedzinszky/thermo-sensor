#include <avr/interrupt.h>
#include <interrupt/EXT_INT0.hpp>

ISR(EXT_INT0_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (EXT_INT0Interrupt::fire_)
	);
}

template <>
volatile bool EXT_INT0Interrupt::fire_ = false;

template <>
callback_t EXT_INT0Interrupt::callback_ = nullptr;
