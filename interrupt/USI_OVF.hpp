#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<USI_OVF_vect_num> USI_OVFInterrupt;
