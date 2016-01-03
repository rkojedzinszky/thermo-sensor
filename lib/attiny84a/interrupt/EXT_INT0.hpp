#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<EXT_INT0_vect_num> EXT_INT0Interrupt;
