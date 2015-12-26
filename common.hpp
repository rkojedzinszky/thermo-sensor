#pragma once

#include <spi/spi_tx5.hpp>
#include <port.hpp>
#include <cc1101.hpp>

typedef CC1101<SPI_tx5, Pin<Port<B>, 3>> radio;

void radio_reset();
