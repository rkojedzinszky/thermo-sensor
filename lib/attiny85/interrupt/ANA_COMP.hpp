#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<ANA_COMP_vect_num> ANA_COMPInterrupt;
