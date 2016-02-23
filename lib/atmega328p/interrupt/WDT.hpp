#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<WDT_vect_num> WDTInterrupt;
