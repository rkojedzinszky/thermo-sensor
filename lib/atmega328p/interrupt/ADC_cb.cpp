#include <avr/interrupt.h>
#include <interrupt/ADC.hpp>

template <>
callback_t ADCInterrupt::callback_ = nullptr;
