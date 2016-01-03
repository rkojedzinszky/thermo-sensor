#include <avr/interrupt.h>
#include <interrupt/USI_STR.hpp>

ISR(USI_STR_vect)
{
	USI_STRInterrupt::tick();
}

template <>
volatile bool USI_STRInterrupt::fire_ = false;

template <>
callback_t USI_STRInterrupt::callback_ = nullptr;
