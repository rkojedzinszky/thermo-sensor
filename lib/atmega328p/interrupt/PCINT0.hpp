#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<PCINT0_vect_num> PCINT0Interrupt;
