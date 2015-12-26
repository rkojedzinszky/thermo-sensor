#include <avr/interrupt.h>
#include <interrupt/USI_START.hpp>

ISR(USI_START_vect)
{
	USI_STARTInterrupt::tick();
}

template <>
volatile bool USI_STARTInterrupt::fire_ = false;

template <>
callback_t USI_STARTInterrupt::callback_ = nullptr;
