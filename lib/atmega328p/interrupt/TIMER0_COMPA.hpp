#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIMER0_COMPA_vect_num> TIMER0_COMPAInterrupt;
