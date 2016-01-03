#include <avr/interrupt.h>
#include <interrupt/TIMER0_COMPB.hpp>

ISR(TIMER0_COMPB_vect)
{
	TIMER0_COMPBInterrupt::tick();
}

template <>
volatile bool TIMER0_COMPBInterrupt::fire_ = false;

template <>
callback_t TIMER0_COMPBInterrupt::callback_ = nullptr;
