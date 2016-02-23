#include <avr/interrupt.h>
#include <interrupt/TIMER1_CAPT.hpp>

template <>
callback_t TIMER1_CAPTInterrupt::callback_ = nullptr;
