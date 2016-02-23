#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<SPI_STC_vect_num> SPI_STCInterrupt;
