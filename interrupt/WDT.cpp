#include <avr/interrupt.h>
#include <interrupt/WDT.hpp>

ISR(WDT_vect)
{
	WDTInterrupt::tick();
}

template <>
volatile bool WDTInterrupt::fire_ = false;

template <>
callback_t WDTInterrupt::callback_ = nullptr;
