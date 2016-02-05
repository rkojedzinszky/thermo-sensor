#include <avr/interrupt.h>
#include <interrupt/EXT_INT0.hpp>

template <>
callback_t EXT_INT0Interrupt::callback_ = nullptr;
