#include <avr/interrupt.h>
#include <interrupt/INT0.hpp>

template <>
callback_t INT0Interrupt::callback_ = nullptr;
