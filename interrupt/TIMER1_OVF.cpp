#include <avr/interrupt.h>
#include <interrupt/TIMER1_OVF.hpp>

ISR(TIMER1_OVF_vect)
{
	TIMER1_OVFInterrupt::tick();
}

template <>
volatile bool TIMER1_OVFInterrupt::fire_ = false;

template <>
callback_t TIMER1_OVFInterrupt::callback_ = nullptr;
