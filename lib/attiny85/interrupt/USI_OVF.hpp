#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<USI_OVF_vect_num> USI_OVFInterrupt;
