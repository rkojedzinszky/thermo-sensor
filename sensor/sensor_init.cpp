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
	if (htu21d->read_temp(d)) {
		seed ^= d;
	}
	if (htu21d->read_hum(d)) {
		seed ^= (d << 8) | (d >> 8);
	}

	return seed;
}

static bool check_reset()
{
	return reset::is_clear() || reset_legacy::is_clear();
}

class HTU21D_dummy : public IHTU21D {
public:
	virtual bool reset();
	virtual bool read_temp(uint16_t& temp);
	virtual bool read_hum(uint16_t& hum);
};

bool HTU21D_dummy::reset()
{
	return false;
}

bool HTU21D_dummy::read_temp(uint16_t&)
{
	return false;
}

bool HTU21D_dummy::read_hum(uint16_t&)
{
	return false;
}

static IHTU21D* detect_htu21d()
{
	static HTU21D<Pin<Port<B>, 1>, Pin<Port<B>, 4>> htu21d_;
	static HTU21D<Pin<Port<B>, 4>, Pin<Port<B>, 1>> htu21d_legacy_;
	static HTU21D_dummy htu21d_dummy_;

	// These two pins are used for bit-bang I2C too
	Pin<Port<B>, 1>::set();
	Pin<Port<B>, 4>::set();

	if (htu21d_.reset())
		return &htu21d_;
	if (htu21d_legacy_.reset())
		return &htu21d_legacy_;

	return &htu21d_dummy_;
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

	htu21d = detect_htu21d();

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

