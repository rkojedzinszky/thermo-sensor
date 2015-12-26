#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<ANA_COMP_vect_num> ANA_COMPInterrupt;
