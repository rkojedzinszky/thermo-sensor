#include <avr/interrupt.h>
#include <interrupt/ADC.hpp>

ISR(ADC_vect)
{
	ADCInterrupt::tick();
}

template <>
volatile bool ADCInterrupt::fire_ = false;

template <>
callback_t ADCInterrupt::callback_ = nullptr;
