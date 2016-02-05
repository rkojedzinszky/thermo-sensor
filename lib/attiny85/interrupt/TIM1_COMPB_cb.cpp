#include <avr/interrupt.h>
#include <interrupt/TIM1_COMPB.hpp>

template <>
callback_t TIM1_COMPBInterrupt::callback_ = nullptr;
