#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor.hpp"

int main()
{
	Sensor sensor;

	PRR = _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRADC);

	sei();

	sensor.init();

	sensor.loop();
}

