#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <config.hpp>
#include <vcc.hpp>
#include "sensor.hpp"
#include "autoconfig.hpp"

static uint16_t getseed()
{
	VCC vcc;

	TCCR0B = _BV(CS00);

	WDTCR = _BV(WDIE);

	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();

	WDTCR = 0;

	uint16_t voltage = vcc.read_voltage();

	return voltage + TCNT0;
}

static bool check_reset()
{
	radio::USI::DO::mode(INPUT);
	radio::USI::DO::set();
	radio::USI::USCK::mode(OUTPUT);
	radio::USI::USCK::clear();

	_NOP();

	bool ret = radio::USI::DO::is_clear();

	radio::USI::USCK::mode(INPUT);

	return ret;
}

void Sensor::init()
{
	Config config;
	config.read();

	bool do_reset = check_reset();

	if (do_reset) {
		config.invalidate();

		for (;;) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}
	}

	radio::setup();
	radio::setup_basic();

	if (!config.valid()) {
		autoconfig(config);
	}

	radio::select();

	for (auto c = config.radioconfig(); c->reg_ != 0xff; ++c) {
		radio::set(c->reg(), c->value);
	}

	radio::setup_for_tx();

	radio::set(CC1101::IOCFG0, 0x06);
	radio::set(CC1101::WORCTRL, 0x7a);

	radio::release();

	magic_ = config.magic();
	id_ = config.id();
	::aes128_init(config.key(), &aes_ctx_);

	lfsr.set(getseed() ^ (config.id() << 8));
}

