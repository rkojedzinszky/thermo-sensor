#include <avr/interrupt.h>
#include <interrupt/PCINT0.hpp>

template <>
callback_t PCINT0Interrupt::callback_ = nullptr;
