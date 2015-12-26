#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<TIMER0_OVF_vect_num> TIMER0_OVFInterrupt;
