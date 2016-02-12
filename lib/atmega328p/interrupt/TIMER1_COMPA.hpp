#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIMER1_COMPA_vect_num> TIMER1_COMPAInterrupt;
