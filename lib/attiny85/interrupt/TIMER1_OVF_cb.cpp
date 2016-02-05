#include <avr/interrupt.h>
#include <interrupt/TIMER1_OVF.hpp>

template <>
callback_t TIMER1_OVFInterrupt::callback_ = nullptr;
