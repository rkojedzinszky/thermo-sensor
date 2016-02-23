#include <avr/interrupt.h>
#include <interrupt/USART_RX.hpp>

template <>
callback_t USART_RXInterrupt::callback_ = nullptr;
