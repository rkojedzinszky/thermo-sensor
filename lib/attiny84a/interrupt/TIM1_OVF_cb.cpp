#include <avr/interrupt.h>
#include <interrupt/TIM1_OVF.hpp>

template <>
callback_t TIM1_OVFInterrupt::callback_ = nullptr;
