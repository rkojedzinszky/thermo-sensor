#include <avr/interrupt.h>
#include <interrupt/WATCHDOG.hpp>

ISR(WATCHDOG_vect)
{
	WATCHDOGInterrupt::tick();
}

template <>
volatile bool WATCHDOGInterrupt::fire_ = false;

template <>
callback_t WATCHDOGInterrupt::callback_ = nullptr;
