#include <avr/interrupt.h>
#include <interrupt/ANALOG_COMP.hpp>

template <>
callback_t ANALOG_COMPInterrupt::callback_ = nullptr;
