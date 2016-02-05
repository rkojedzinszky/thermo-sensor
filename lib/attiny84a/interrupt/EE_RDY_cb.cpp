#include <avr/interrupt.h>
#include <interrupt/EE_RDY.hpp>

template <>
callback_t EE_RDYInterrupt::callback_ = nullptr;
