#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<EE_RDY_vect_num> EE_RDYInterrupt;
