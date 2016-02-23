#include <avr/interrupt.h>
#include <interrupt/SPI_STC.hpp>

ISR(SPI_STC_vect, ISR_NAKED)
{
	asm(
		"push r24\n"
		"ldi r24, 1\n"
		"sts %0, r24\n"
		"pop r24\n"
		"reti\n"
		:
		: "m" (SPI_STCInterrupt::fire_)
	);
}

template <>
volatile bool SPI_STCInterrupt::fire_ = false;
