#include <avr/interrupt.h>
#include <interrupt/TIM0_COMPB.hpp>

template <>
callback_t TIM0_COMPBInterrupt::callback_ = nullptr;
