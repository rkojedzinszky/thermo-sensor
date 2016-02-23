#include <avr/interrupt.h>
#include <interrupt/PCINT2.hpp>

template <>
callback_t PCINT2Interrupt::callback_ = nullptr;
