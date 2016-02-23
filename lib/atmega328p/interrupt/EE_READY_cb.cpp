#include <avr/interrupt.h>
#include <interrupt/EE_READY.hpp>

template <>
callback_t EE_READYInterrupt::callback_ = nullptr;
