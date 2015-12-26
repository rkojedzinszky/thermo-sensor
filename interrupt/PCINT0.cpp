#include <avr/interrupt.h>
#include <interrupt/PCINT0.hpp>

ISR(PCINT0_vect)
{
	PCINT0Interrupt::tick();
}

template <>
volatile bool PCINT0Interrupt::fire_ = false;

template <>
callback_t PCINT0Interrupt::callback_ = nullptr;
