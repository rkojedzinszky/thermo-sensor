#include <avr/interrupt.h>
#include <interrupt/TIMER0_COMPA.hpp>

template <>
callback_t TIMER0_COMPAInterrupt::callback_ = nullptr;
