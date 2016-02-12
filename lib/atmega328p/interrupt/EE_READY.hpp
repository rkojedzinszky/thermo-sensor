#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<EE_READY_vect_num> EE_READYInterrupt;
