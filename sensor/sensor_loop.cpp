#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/PCINT0.hpp>
#include "sensor.hpp"

// 26 + 2 + [0, 2) seconds = [28, 30) seconds
static constexpr int delay_1 = 26;
static constexpr int delay_2 = 2;

static constexpr unsigned int delay_1_ticks = delay_1 * delay_multiplier;
static constexpr unsigned int delay_2_ticks = delay_2 * delay_multiplier;

void Sensor::loop()
{
	PCMSK = _BV(radio::USI::DI::pin);

	for (;;) {
		uint16_t ticks = delay_1_ticks + (lfsr.get() & 0x3f);
		radio::select();
		radio::set(CC1101::WOREVT1, ticks >> 8);
		radio::set(CC1101::WOREVT0, ticks & 0xff);
		radio::set(CC1101::IOCFG1, 0xa4);
		radio::wcmd(CC1101::SPWD);
		radio::release();

		GIMSK |= _BV(PCIE);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		GIMSK &= ~_BV(PCIE);

		radio::select();
		_thermo_on(true);
		radio::set(CC1101::WOREVT1, delay_2_ticks >> 8);
		radio::set(CC1101::WOREVT0, delay_2_ticks & 0xff);
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

