#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/PCINT0.hpp>
#include "sensor.hpp"

// 28 + [0, 2) seconds = [28, 30) seconds
static constexpr int delay_1 = 28;

static constexpr unsigned int delay_1_ticks = delay_1 * delay_multiplier;

void Sensor::loop()
{
	for (;;) {
		uint16_t ticks = delay_1_ticks + (lfsr.get() & 0x3f);
		radio::select();
		radio::set(CC1101::WOREVT1, ticks >> 8);
		radio::set(CC1101::WOREVT0, ticks & 0xff);
		radio::set(CC1101::IOCFG1, 0xa4);
		radio::wcmd(CC1101::SPWD);
		radio::release();

		PCMSK = _BV(radio::USI::DI::pin);
		GIMSK |= _BV(PCIE);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		GIMSK &= ~_BV(PCIE);

		send();

		PCMSK = _BV(radio::USI::DI::pin);
		GIMSK |= _BV(PCIE);
		while (radio::USI::DI::is_set()) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}
		GIMSK &= ~_BV(PCIE);
	}
}

