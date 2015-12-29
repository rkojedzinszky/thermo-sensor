#pragma once

#include <spi/spi_t84a.hpp>
#include <port.hpp>
#include <cc1101.hpp>

typedef CC1101<SPI_t84a, Pin<Port<A>, 7>> radio;

void radio_reset();
