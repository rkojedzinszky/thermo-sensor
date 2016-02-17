#pragma once

#include <port.hpp>
#include <spi.hpp>
#include <usart.hpp>
#include <radio.hpp>

typedef Radio<CC1101::CC1101<SPI, Pin<Port<B>, 2>>> radio;
typedef Usart<0, 5> usart;

