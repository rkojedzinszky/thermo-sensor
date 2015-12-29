#include <avr/interrupt.h>
#include <interrupt/PCINT1.hpp>

ISR(PCINT1_vect)
{
	PCINT1Interrupt::tick();
}

template <>
volatile bool PCINT1Interrupt::fire_ = false;

template <>
callback_t PCINT1Interrupt::callback_ = nullptr;
