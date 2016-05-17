#pragma once

#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/ADC.hpp>
#include <interrupt/TIM0_COMPA.hpp>

class VCC {
public:
	static unsigned read_voltage();

private:
	static void interrupt();
	static bool done_;
};
