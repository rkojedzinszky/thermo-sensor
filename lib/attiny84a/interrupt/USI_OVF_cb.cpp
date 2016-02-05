#include <avr/interrupt.h>
#include <interrupt/USI_OVF.hpp>

template <>
callback_t USI_OVFInterrupt::callback_ = nullptr;
