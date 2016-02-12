#include <avr/interrupt.h>
#include <interrupt/SPI_STC.hpp>

template <>
callback_t SPI_STCInterrupt::callback_ = nullptr;
