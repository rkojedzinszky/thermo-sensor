#include <avr/interrupt.h>
#include <interrupt/SPM_READY.hpp>

template <>
callback_t SPM_READYInterrupt::callback_ = nullptr;
