#include <avr/interrupt.h>
#include <interrupt/TIMER1_COMPA.hpp>

ISR(TIMER1_COMPA_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (TIMER1_COMPAInterrupt::fire_)
	);
}

template <>
volatile bool TIMER1_COMPAInterrupt::fire_ = false;

template <>
callback_t TIMER1_COMPAInterrupt::callback_ = nullptr;
