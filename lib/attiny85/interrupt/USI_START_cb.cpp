#include <avr/interrupt.h>
#include <interrupt/USI_START.hpp>

template <>
callback_t USI_STARTInterrupt::callback_ = nullptr;
