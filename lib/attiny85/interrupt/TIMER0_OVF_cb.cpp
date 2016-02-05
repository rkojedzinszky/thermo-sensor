#include <avr/interrupt.h>
#include <interrupt/TIMER0_OVF.hpp>

template <>
callback_t TIMER0_OVFInterrupt::callback_ = nullptr;
