#include <avr/interrupt.h>
#include <interrupt/TIM0_COMPA.hpp>

template <>
callback_t TIM0_COMPAInterrupt::callback_ = nullptr;
