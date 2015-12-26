#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<WDT_vect_num> WDTInterrupt;
