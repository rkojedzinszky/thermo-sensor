#include <avr/interrupt.h>
#include <interrupt/TIMER1_COMPB.hpp>

template <>
callback_t TIMER1_COMPBInterrupt::callback_ = nullptr;
