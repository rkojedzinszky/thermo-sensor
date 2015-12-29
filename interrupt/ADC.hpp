#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<ADC_vect_num> ADCInterrupt;
