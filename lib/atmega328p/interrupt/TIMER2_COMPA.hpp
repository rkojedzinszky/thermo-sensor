#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIMER2_COMPA_vect_num> TIMER2_COMPAInterrupt;
