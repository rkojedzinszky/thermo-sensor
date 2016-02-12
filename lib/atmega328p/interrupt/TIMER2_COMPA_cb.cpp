#include <avr/interrupt.h>
#include <interrupt/TIMER2_COMPA.hpp>

template <>
callback_t TIMER2_COMPAInterrupt::callback_ = nullptr;
