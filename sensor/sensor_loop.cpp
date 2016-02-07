#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/WDT.hpp>
#include <interrupt/PCINT0.hpp>
#include "sensor.hpp"

static constexpr int wdt_p_timeouts = 5;

static void radio_off()
{
	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();
}

// do a measurement and sleep a wdt tick (4 seconds)
static void do_measure()
{
	short thum, ttemp;

	am2302::read(thum, ttemp);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
}

void Sensor::loop()
{
	PCMSK = _BV(radio::USI::DI::pin);

	for (;;) {
		radio_off();

		WDTCR = random_wdt_delay();
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();

		WDTCR = _BV(WDIE) | _BV(WDP3); // 4 secs
		for (uint8_t i = 0; i < wdt_p_timeouts; ++i) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}

		do_measure();

		send();

		WDTInterrupt::fire_ = false;
		PCINT0Interrupt::fire_ = false;
		GIMSK |= _BV(PCIE);

		while (WDTInterrupt::fire_ == false && radio::USI::DI::is_set()) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}

		GIMSK &= ~_BV(PCIE);
	}
}

