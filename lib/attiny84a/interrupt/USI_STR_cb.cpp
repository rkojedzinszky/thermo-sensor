#include <avr/interrupt.h>
#include <interrupt/USI_STR.hpp>

template <>
callback_t USI_STRInterrupt::callback_ = nullptr;
