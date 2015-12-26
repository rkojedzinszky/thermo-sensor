#include <avr/interrupt.h>
#include <interrupt/TIM1_COMPB.hpp>

ISR(TIM1_COMPB_vect)
{
	TIM1_COMPBInterrupt::tick();
}

template <>
volatile bool TIM1_COMPBInterrupt::fire_ = false;

template <>
callback_t TIM1_COMPBInterrupt::callback_ = nullptr;
