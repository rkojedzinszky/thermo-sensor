#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<TIM1_CAPT_vect_num> TIM1_CAPTInterrupt;
