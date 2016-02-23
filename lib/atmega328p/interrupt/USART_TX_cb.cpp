#include <avr/interrupt.h>
#include <interrupt/USART_TX.hpp>

template <>
callback_t USART_TXInterrupt::callback_ = nullptr;
