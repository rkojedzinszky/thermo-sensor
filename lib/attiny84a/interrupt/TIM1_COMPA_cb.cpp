#include <avr/interrupt.h>
#include <interrupt/TIM1_COMPA.hpp>

template <>
callback_t TIM1_COMPAInterrupt::callback_ = nullptr;
