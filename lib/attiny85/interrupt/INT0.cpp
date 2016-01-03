#include <avr/interrupt.h>
#include <interrupt/INT0.hpp>

ISR(INT0_vect)
{
	INT0Interrupt::tick();
}

template <>
volatile bool INT0Interrupt::fire_ = false;

template <>
callback_t INT0Interrupt::callback_ = nullptr;
