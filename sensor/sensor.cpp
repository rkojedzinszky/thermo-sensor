#include <avr/cpufunc.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

extern "C" {
#include <aes/aes.h>
};

#include <lfsr.hpp>
#include <port.hpp>
#include <interrupt/WDT.hpp>
#include <sensorvalue.hpp>
#include <vcc.hpp>
#include <common.hpp>
#include <config.hpp>
#include <packet.hpp>
#include "sensor.hpp"
#include "autoconfig.hpp"

static LFSR<7> lfsr;
static constexpr int wdt_p_timeouts = 5;
static unsigned short magic;
static unsigned char id;
static aes128_ctx_t aes_ctx;

static void lfsr_init(Config& config)/*{{{*/
{
	VCC vcc;

	TCCR0B = _BV(CS00);

	WDTCR = _BV(WDIE);

	sei();

	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();

	WDTCR = 0;

	uint16_t voltage = vcc.read_voltage();
	cli();

	lfsr.set(TCNT0 ^ (voltage & 0xff) ^ (voltage >> 8) ^ config.id());

	WDTCR = 0;

	TCCR0B = 0;
}/*}}}*/

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

void init()
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

	lfsr_init(config);

	radio::select();

	for (auto c = config.radioconfig(); c->reg_ != 0xff; ++c) {
		radio::set(c->reg(), c->value);
	}

	radio::setup_for_tx();

	radio::set(CC1101::IOCFG1, 0x06);
	radio::set(CC1101::IOCFG0, 0x06);

	radio::release();

	magic = config.magic();
	id = config.id();
	aes128_init(config.key(), &aes_ctx);
}

static void do_send()
{
	static unsigned short seq_ = 0;
	Radiopacket packet;

	VCC vccreader;
	unsigned char* dp = packet.data;

	short hum, temp;
	if (am2302::read(hum, temp)) {
		dp += SensorValue<Humidity>::encode(hum, dp);
		dp += SensorValue<Temperature>::encode(temp, dp);
	}

	unsigned vl = vccreader.read_voltage();
	dp += SensorValue<Power>::encode(vl, dp);

	packet.magic = magic;
	packet.len = dp - packet.raw;
	packet.id = id;
	packet.seq = seq_++;

	aes128_enc(packet.raw, &aes_ctx);

	radio::select();
	radio::wcmd(CC1101::STX);
	radio::write_txfifo(packet.raw, sizeof(packet.raw));
}

static void radio_off()
{
	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();
}

static uint8_t random_wdt_delay()
{
	uint8_t rnd = lfsr.get() & 7;

	if (rnd == 7) {
		rnd = _BV(WDIE) | _BV(WDP3); // 4 sec
	} else {
		rnd = _BV(WDIE) | (rnd + 1);
	}

	return rnd;
}

// do a measurement and sleep a wdt tick (4 seconds)
static void do_measure()
{
	short thum, ttemp;

	am2302::read(thum, ttemp);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
}

int main()
{
	init();

	sei();

	PRR = _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRADC);

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

		do_send();

		WDTInterrupt::fire_ = false;
		GIMSK |= _BV(PCIE);

		while (WDTInterrupt::fire_ == false && radio::USI::DI::is_set()) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}

		GIMSK &= ~_BV(PCIE);
	}
}

template uint8_t CC1101::CC1101<USI, Pin<Port<B>, 3>>::select();
template void CC1101::CC1101<USI, Pin<Port<B>, 3>>::write_txfifo(uint8_t* values, uint8_t len);
template void CC1101::CC1101<USI, Pin<Port<B>, 3>>::read_rxfifo(uint8_t* values, uint8_t len);
