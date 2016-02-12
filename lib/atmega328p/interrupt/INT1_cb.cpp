#include <avr/interrupt.h>
#include <interrupt/INT1.hpp>

template <>
callback_t INT1Interrupt::callback_ = nullptr;
