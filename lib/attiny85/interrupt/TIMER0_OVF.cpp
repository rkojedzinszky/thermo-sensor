#include <avr/interrupt.h>
#include <interrupt/TIMER0_OVF.hpp>

ISR(TIMER0_OVF_vect)
{
	TIMER0_OVFInterrupt::tick();
}

template <>
volatile bool TIMER0_OVFInterrupt::fire_ = false;

template <>
callback_t TIMER0_OVFInterrupt::callback_ = nullptr;
