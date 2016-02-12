#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<PCINT2_vect_num> PCINT2Interrupt;
