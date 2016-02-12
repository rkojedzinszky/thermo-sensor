#include <avr/interrupt.h>
#include <interrupt/TWI.hpp>

template <>
callback_t TWIInterrupt::callback_ = nullptr;
