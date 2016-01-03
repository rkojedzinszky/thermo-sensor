#include <avr/interrupt.h>
#include <interrupt/USI_OVF.hpp>

ISR(USI_OVF_vect)
{
	USI_OVFInterrupt::tick();
}

template <>
volatile bool USI_OVFInterrupt::fire_ = false;

template <>
callback_t USI_OVFInterrupt::callback_ = nullptr;
