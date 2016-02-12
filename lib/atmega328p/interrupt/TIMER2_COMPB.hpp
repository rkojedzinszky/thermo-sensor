#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIMER2_COMPB_vect_num> TIMER2_COMPBInterrupt;
