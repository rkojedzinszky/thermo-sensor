#include <avr/interrupt.h>
#include <interrupt/ANA_COMP.hpp>

ISR(ANA_COMP_vect)
{
	ANA_COMPInterrupt::tick();
}

template <>
volatile bool ANA_COMPInterrupt::fire_ = false;

template <>
callback_t ANA_COMPInterrupt::callback_ = nullptr;
