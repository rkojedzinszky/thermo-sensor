#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIMER1_COMPB_vect_num> TIMER1_COMPBInterrupt;
