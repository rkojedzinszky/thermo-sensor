#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<ANALOG_COMP_vect_num> ANALOG_COMPInterrupt;
