#include <avr/interrupt.h>
#include <interrupt/TIMER1_COMPA.hpp>

ISR(TIMER1_COMPA_vect)
{
	TIMER1_COMPAInterrupt::tick();
}

template <>
volatile bool TIMER1_COMPAInterrupt::fire_ = false;

template <>
callback_t TIMER1_COMPAInterrupt::callback_ = nullptr;
