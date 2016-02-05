#include <avr/interrupt.h>
#include <interrupt/WATCHDOG.hpp>

template <>
callback_t WATCHDOGInterrupt::callback_ = nullptr;
