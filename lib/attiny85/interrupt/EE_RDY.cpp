#include <avr/interrupt.h>
#include <interrupt/EE_RDY.hpp>

ISR(EE_RDY_vect)
{
	EE_RDYInterrupt::tick();
}

template <>
volatile bool EE_RDYInterrupt::fire_ = false;

template <>
callback_t EE_RDYInterrupt::callback_ = nullptr;
