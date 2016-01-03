#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIM1_COMPB_vect_num> TIM1_COMPBInterrupt;
