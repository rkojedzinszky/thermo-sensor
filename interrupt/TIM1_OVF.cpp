#include <avr/interrupt.h>
#include <interrupt/TIM1_OVF.hpp>

ISR(TIM1_OVF_vect)
{
	TIM1_OVFInterrupt::tick();
}

template <>
volatile bool TIM1_OVFInterrupt::fire_ = false;

template <>
callback_t TIM1_OVFInterrupt::callback_ = nullptr;
