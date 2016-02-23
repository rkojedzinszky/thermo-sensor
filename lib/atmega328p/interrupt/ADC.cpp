#include <avr/interrupt.h>
#include <interrupt/ADC.hpp>

ISR(ADC_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (ADCInterrupt::fire_)
	);
}

template <>
volatile bool ADCInterrupt::fire_ = false;
