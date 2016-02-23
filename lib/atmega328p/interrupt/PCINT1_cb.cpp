#include <avr/interrupt.h>
#include <interrupt/PCINT1.hpp>

template <>
callback_t PCINT1Interrupt::callback_ = nullptr;
