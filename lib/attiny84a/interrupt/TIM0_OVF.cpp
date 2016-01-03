#include <avr/interrupt.h>
#include <interrupt/TIM0_OVF.hpp>

ISR(TIM0_OVF_vect)
{
	TIM0_OVFInterrupt::tick();
}

template <>
volatile bool TIM0_OVFInterrupt::fire_ = false;

template <>
callback_t TIM0_OVFInterrupt::callback_ = nullptr;
