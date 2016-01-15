#include <avr/interrupt.h>
#include <interrupt/WDT.hpp>

ISR(WDT_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (WDTInterrupt::fire_)
	);
}

template <>
volatile bool WDTInterrupt::fire_ = false;

template <>
callback_t WDTInterrupt::callback_ = nullptr;
