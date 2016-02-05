#include <avr/interrupt.h>
#include <interrupt/TIM1_CAPT.hpp>

template <>
callback_t TIM1_CAPTInterrupt::callback_ = nullptr;
