#include <avr/interrupt.h>
#include <interrupt/TIM0_OVF.hpp>

template <>
callback_t TIM0_OVFInterrupt::callback_ = nullptr;
