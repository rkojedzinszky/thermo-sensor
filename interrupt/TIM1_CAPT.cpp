#include <avr/interrupt.h>
#include <interrupt/TIM1_CAPT.hpp>

ISR(TIM1_CAPT_vect)
{
	TIM1_CAPTInterrupt::tick();
}

template <>
volatile bool TIM1_CAPTInterrupt::fire_ = false;

template <>
callback_t TIM1_CAPTInterrupt::callback_ = nullptr;
