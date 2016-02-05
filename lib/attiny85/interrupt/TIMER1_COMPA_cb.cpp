#include <avr/interrupt.h>
#include <interrupt/TIMER1_COMPA.hpp>

template <>
callback_t TIMER1_COMPAInterrupt::callback_ = nullptr;
