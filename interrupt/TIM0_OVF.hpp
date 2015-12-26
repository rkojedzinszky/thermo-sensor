#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<TIM0_OVF_vect_num> TIM0_OVFInterrupt;
