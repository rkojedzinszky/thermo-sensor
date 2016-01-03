#include <avr/interrupt.h>
#include <interrupt/TIM0_COMPB.hpp>

ISR(TIM0_COMPB_vect)
{
	TIM0_COMPBInterrupt::tick();
}

template <>
volatile bool TIM0_COMPBInterrupt::fire_ = false;

template <>
callback_t TIM0_COMPBInterrupt::callback_ = nullptr;
