#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/PCINT0.hpp>
#include "sensor.hpp"

static constexpr int delay_1 = 26;
static constexpr int delay_2 = 2;

static constexpr unsigned int delay_1h = (delay_1 * 1083) >> 8;
static constexpr unsigned int delay_1l = (delay_1 * 1083) & 0xff;

static constexpr unsigned int delay_2h = (delay_2 * 1083) >> 8;
static constexpr unsigned int delay_2l = (delay_2 * 1083) & 0xff;

void Sensor::loop()
{
	PCMSK = _BV(radio::USI::DI::pin);

	for (;;) {
		radio::select();
		radio::set(CC1101::WOREVT1, delay_1h);
		radio::set(CC1101::WOREVT0, delay_1l);
		radio::set(CC1101::WORCTRL, 0x71);
		radio::set(CC1101::IOCFG1, 0xa4);
		radio::wcmd(CC1101::SPWD);
		radio::release();

		GIMSK |= _BV(PCIE);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		GIMSK &= ~_BV(PCIE);

		radio::select();
		_thermo_on(true);
		radio::set(CC1101::WOREVT1, delay_2h);
		radio::set(CC1101::WOREVT0, delay_2l);
		radio::set(CC1101::MCSM0, 0x38);
		radio::wcmd(CC1101::SPWD);
		radio::release();

		GIMSK |= _BV(PCIE);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		GIMSK &= ~_BV(PCIE);

		send();

		GIMSK |= _BV(PCIE);
		while (radio::USI::DI::is_set()) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}
		GIMSK &= ~_BV(PCIE);
	}
}

