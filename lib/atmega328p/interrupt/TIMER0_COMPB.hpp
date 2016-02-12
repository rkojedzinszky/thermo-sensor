#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIMER0_COMPB_vect_num> TIMER0_COMPBInterrupt;
