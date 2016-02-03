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
#include <interrupt/PCINT0.hpp>
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

static bool _check_reset()
{
	for (uint8_t i = 0; i < 10; ++i) {
		radio::USI::USCK::clear();
		_NOP();
		_NOP();
		if (!radio::USI::DO::is_clear()) {
			return false;
		}

		radio::USI::USCK::set();
		_NOP();
		_NOP();
		if (!radio::USI::DO::is_set()) {
			return false;
		}
	}

	return true;
}

static bool check_reset()
{
	radio::USI::DO::mode(INPUT);
	radio::USI::DO::set();
	radio::USI::USCK::mode(OUTPUT);

	bool ret = _check_reset();

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

static void send()
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

static void loop();

static void random_timeout_cb()
{
	WDTCR = _BV(WDIE) | _BV(WDP3); // 4 secs
	WDTInterrupt::set(loop);
}

static void radio_off()
{
	GIMSK &= ~_BV(PCIE);

	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();

	WDTCR = _BV(WDIE) | ((lfsr.get() & 7) + 1); // 32ms - 4s
	WDTInterrupt::set(random_timeout_cb);
}

static void pcint()
{
	if (radio::USI::DI::is_clear()) {
		radio_off();
	}
}

static void loop()
{
	static unsigned char counter = 0;

	counter++;

	if (counter == wdt_p_timeouts) {
		short thum, ttemp;
		am2302::read(thum, ttemp);
	} else if (counter == wdt_p_timeouts + 1) {
		counter = 0;
		send();

		WDTInterrupt::set(radio_off);

		GIMSK |= _BV(PCIE);
	}
}

int main()
{
	init();

	sei();

	PCMSK |= _BV(radio::USI::DI::pin);
	PCINT0Interrupt::set(pcint);

	radio_off();

	for (;;) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		PCINT0Interrupt::pending();
		WDTInterrupt::pending();
	}
}
