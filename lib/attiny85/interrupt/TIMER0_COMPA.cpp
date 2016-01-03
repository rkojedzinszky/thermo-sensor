#include <avr/interrupt.h>
#include <interrupt/TIMER0_COMPA.hpp>

ISR(TIMER0_COMPA_vect)
{
	TIMER0_COMPAInterrupt::tick();
}

template <>
volatile bool TIMER0_COMPAInterrupt::fire_ = false;

template <>
callback_t TIMER0_COMPAInterrupt::callback_ = nullptr;
