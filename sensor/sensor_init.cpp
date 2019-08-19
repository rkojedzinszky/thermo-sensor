#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <config.hpp>
#include <vcc.hpp>
#include "sensor.hpp"
#include "autoconfig.hpp"

static bool check_reset()
{
	return reset::is_clear() || reset_legacy::is_clear();
}

uint16_t Sensor::getseed()
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
	if (HTU21D::read_temp(d)) {
		seed ^= d;
	}
	if (HTU21D::read_hum(d)) {
		seed ^= (d << 8) | (d >> 8);
	}

	return seed;
}

void Sensor::detect_htu21d()
{
	static I2C<Pin<Port<B>, 1>, Pin<Port<B>, 4>> i2c;
	static I2C<Pin<Port<B>, 4>, Pin<Port<B>, 1>> i2c_legacy;

	// These two pins are used for bit-bang I2C too
	Pin<Port<B>, 1>::set();
	Pin<Port<B>, 4>::set();

	HTU21D::seti2c(&i2c);
	if (HTU21D::reset() || HTU21D::reset())
		return;

	HTU21D::seti2c(&i2c_legacy);
	if (HTU21D::reset() || HTU21D::reset())
		return;

	HTU21D::seti2c(0);
}

void Sensor::init()
{
	reset::set();
	reset_legacy::set();

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

		{ // sleep for 4 seconds/*{{{*/
			wdt_reset();
			WDTCR = _BV(WDIE) | _BV(WDP3);

			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sei();
			sleep_mode();
			cli();

			WDTCR = 0;
		}/*}}}*/

		config.invalidate_id();

		wdt_reset();
		WDTCR = _BV(WDIE) | _BV(WDP2) | _BV(WDP0); // blink every second
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

	detect_htu21d();

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

