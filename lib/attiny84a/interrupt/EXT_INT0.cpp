#include <avr/interrupt.h>
#include <interrupt/EXT_INT0.hpp>

ISR(EXT_INT0_vect)
{
	EXT_INT0Interrupt::tick();
}

template <>
volatile bool EXT_INT0Interrupt::fire_ = false;

template <>
callback_t EXT_INT0Interrupt::callback_ = nullptr;
