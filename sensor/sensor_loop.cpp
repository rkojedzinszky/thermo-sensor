#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/WDT.hpp>
#include <interrupt/PCINT0.hpp>
#include "sensor.hpp"

static constexpr int wdt_p_timeouts = 3;

static void radio_off()
{
	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();
}

void Sensor::loop()
{
	PCMSK = _BV(radio::USI::DI::pin);

	for (;;) {
		radio_off();

		WDTCR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0); // 8 secs
		for (uint8_t i = 0; i < wdt_p_timeouts; ++i) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}

		thermo_on(true);

		WDTCR = _BV(WDIE) | _BV(WDP3); // 4 secs
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();

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

