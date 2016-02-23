#include <avr/interrupt.h>
#include <interrupt/WDT.hpp>

template <>
callback_t WDTInterrupt::callback_ = nullptr;
