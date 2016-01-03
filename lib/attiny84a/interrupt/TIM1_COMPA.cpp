#include <avr/interrupt.h>
#include <interrupt/TIM1_COMPA.hpp>

ISR(TIM1_COMPA_vect)
{
	TIM1_COMPAInterrupt::tick();
}

template <>
volatile bool TIM1_COMPAInterrupt::fire_ = false;

template <>
callback_t TIM1_COMPAInterrupt::callback_ = nullptr;
