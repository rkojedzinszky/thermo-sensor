#include <avr/interrupt.h>
#include <interrupt/TIMER2_OVF.hpp>

template <>
callback_t TIMER2_OVFInterrupt::callback_ = nullptr;
