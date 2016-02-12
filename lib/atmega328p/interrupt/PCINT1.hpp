#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<PCINT1_vect_num> PCINT1Interrupt;
