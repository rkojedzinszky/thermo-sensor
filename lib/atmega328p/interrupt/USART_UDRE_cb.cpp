#include <avr/interrupt.h>
#include <interrupt/USART_UDRE.hpp>

template <>
callback_t USART_UDREInterrupt::callback_ = nullptr;
