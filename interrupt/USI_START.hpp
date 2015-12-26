#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<USI_START_vect_num> USI_STARTInterrupt;
