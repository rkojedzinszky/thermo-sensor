#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TWI_vect_num> TWIInterrupt;
