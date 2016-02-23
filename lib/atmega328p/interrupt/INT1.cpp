#include <avr/interrupt.h>
#include <interrupt/INT1.hpp>

ISR(INT1_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (INT1Interrupt::fire_)
	);
}

template <>
volatile bool INT1Interrupt::fire_ = false;
