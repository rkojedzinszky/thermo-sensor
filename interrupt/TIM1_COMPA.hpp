#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<TIM1_COMPA_vect_num> TIM1_COMPAInterrupt;