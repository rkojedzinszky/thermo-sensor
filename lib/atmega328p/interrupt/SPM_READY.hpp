#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<SPM_READY_vect_num> SPM_READYInterrupt;
