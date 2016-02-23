#include <avr/interrupt.h>
#include <interrupt/TIMER0_COMPB.hpp>

template <>
callback_t TIMER0_COMPBInterrupt::callback_ = nullptr;
