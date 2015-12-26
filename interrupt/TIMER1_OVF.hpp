#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<TIMER1_OVF_vect_num> TIMER1_OVFInterrupt;
