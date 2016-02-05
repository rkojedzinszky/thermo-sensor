#include <avr/interrupt.h>
#include <interrupt/ANA_COMP.hpp>

template <>
callback_t ANA_COMPInterrupt::callback_ = nullptr;
