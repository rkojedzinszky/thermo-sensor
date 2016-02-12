#include <avr/interrupt.h>
#include <interrupt/TIMER2_COMPB.hpp>

template <>
callback_t TIMER2_COMPBInterrupt::callback_ = nullptr;
