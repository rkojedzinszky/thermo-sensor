#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<USI_STR_vect_num> USI_STRInterrupt;
