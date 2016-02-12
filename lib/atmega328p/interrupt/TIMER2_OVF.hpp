#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIMER2_OVF_vect_num> TIMER2_OVFInterrupt;
