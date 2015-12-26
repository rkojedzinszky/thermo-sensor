#include <avr/interrupt.h>
#include <interrupt/TIMER1_COMPB.hpp>

ISR(TIMER1_COMPB_vect)
{
	TIMER1_COMPBInterrupt::tick();
}

template <>
volatile bool TIMER1_COMPBInterrupt::fire_ = false;

template <>
callback_t TIMER1_COMPBInterrupt::callback_ = nullptr;
