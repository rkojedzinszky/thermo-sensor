#include <avr/interrupt.h>
#include <interrupt/WATCHDOG.hpp>

ISR(WATCHDOG_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (WATCHDOGInterrupt::fire_)
	);
}

template <>
volatile bool WATCHDOGInterrupt::fire_ = false;
