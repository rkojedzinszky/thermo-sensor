#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor.hpp"

int main()
{
	PRR = _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRADC);
	ACSR = _BV(ACD);
	DIDR0 = _BV(AIN1D) | _BV(AIN0D);

	sei();

	radio::setup();
	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();

	// Initial 4 sec sleep before anything for capacitor to be charged
	wdt_reset();
	WDTCR = _BV(WDIE) | _BV(WDP3);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	WDTCR = 0;

	Sensor sensor;

	sensor.init();

	sensor.loop();
}

