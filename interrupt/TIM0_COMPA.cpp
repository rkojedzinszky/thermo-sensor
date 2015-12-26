#include <avr/interrupt.h>
#include <interrupt/TIM0_COMPA.hpp>

ISR(TIM0_COMPA_vect)
{
	TIM0_COMPAInterrupt::tick();
}

template <>
volatile bool TIM0_COMPAInterrupt::fire_ = false;

template <>
callback_t TIM0_COMPAInterrupt::callback_ = nullptr;
