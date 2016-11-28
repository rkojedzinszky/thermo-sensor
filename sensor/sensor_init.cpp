#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <config.hpp>
#include <vcc.hpp>
#include "sensor.hpp"
#include "autoconfig.hpp"

static uint16_t getseed()
{
	VCC vcc;

	TCCR0B = _BV(CS00);

	wdt_reset();
	WDTCR = _BV(WDIE);

	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();

	WDTCR = 0;

	uint16_t seed = TCNT0 << 8;
	seed ^= vcc.read_voltage();
	// read_voltage stops timer also

	USICR = 0;
	uint16_t d;
	if (htu21d::read_temp(d)) {
		seed ^= d;
	}
	if (htu21d::read_hum(d)) {
		seed ^= (d << 8) | (d >> 8);
	}

	return seed;
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
	radio::USI::USCK::set();

	return ret;
}

void Sensor::init()
{
	htu21d::reset();

	Config config;
	config.read();

	bool do_reset = check_reset();

	radio::setup();

	if (do_reset) {
		cli();

		config.invalidate();

		radio::select();
		radio::set(CC1101::IOCFG0, 0x6f);
		radio::wcmd(CC1101::SPWD);
		radio::release();

		wdt_reset();
		WDTCR = _BV(WDIE) | _BV(WDP3);

		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sei();
		sleep_mode();
		cli();

		WDTCR = 0;

		config.invalidate_id();

		wdt_reset();
		WDTCR = _BV(WDIE) | _BV(WDP2) | _BV(WDP1) | _BV(WDP0);
		sei();

		for (;;) {
			radio::select();
			radio::set(CC1101::IOCFG0, radio::get(CC1101::IOCFG0) ^ 0x40);
			radio::wcmd(CC1101::SPWD);
			radio::release();

			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}
	}

	htu21d::CL::set();
	htu21d::DA::set();

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

	radio::wcmd(CC1101::SPWD);
	radio::release();

	magic_ = config.magic();
	id_ = config.id();
	::aes128_init(config.key(), &aes_ctx_);

	lfsr.set(getseed() ^ (config.id() << 8));
	seq_ = ((static_cast<uint32_t>(lfsr.get()) << 16) | lfsr.get()) & 0x7fffffff;
}

