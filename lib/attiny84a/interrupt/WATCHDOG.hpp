#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<WATCHDOG_vect_num> WATCHDOGInterrupt;
