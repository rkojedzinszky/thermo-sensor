#pragma once

#include <avr/io.h>
#include <interrupt/base.hpp>

typedef Interrupt<ADC_vect_num> ADCInterrupt;
