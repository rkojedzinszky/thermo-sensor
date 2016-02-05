#include <avr/cpufunc.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <port.hpp>
#include <interrupt/WDT.hpp>
#include <sensorvalue.hpp>
#include <vcc.hpp>
#include <common.hpp>
#include <config.hpp>
#include <packet.hpp>
#include "sensor.hpp"
#include "autoconfig.hpp"

static constexpr int wdt_p_timeouts = 5;

static uint8_t getseed()/*{{{*/
{
	VCC vcc;

	TCCR0B = _BV(CS00);

	WDTCR = _BV(WDIE);

	sei();

	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();

	WDTCR = 0;

	uint16_t voltage = vcc.read_voltage();

	return TCNT0 ^ (voltage & 0xff) ^ (voltage >> 8);
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

void init(Main& main)
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

	main.seedlfsr(getseed() ^ config.id());

	radio::select();

	for (auto c = config.radioconfig(); c->reg_ != 0xff; ++c) {
		radio::set(c->reg(), c->value);
	}

	radio::setup_for_tx();

	radio::set(CC1101::IOCFG1, 0x06);
	radio::set(CC1101::IOCFG0, 0x06);

	radio::release();

	main.setmagic(config.magic());
	main.setid(config.id());
	main.setkey(config.key());
}

static void radio_off()
{
	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();
}

// do a measurement and sleep a wdt tick (4 seconds)
static void do_measure()
{
	short thum, ttemp;

	am2302::read(thum, ttemp);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
}

void Main::loop()
{
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

		send();

		WDTInterrupt::fire_ = false;
		GIMSK |= _BV(PCIE);

		while (WDTInterrupt::fire_ == false && radio::USI::DI::is_set()) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}

		GIMSK &= ~_BV(PCIE);
	}
}

inline void Main::send()
{
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

	packet.magic = magic_;
	packet.len = dp - packet.raw;
	packet.id = id_;
	packet.seq = seq_++;

	::aes128_enc(packet.raw, &aes_ctx_);

	radio::select();
	radio::wcmd(CC1101::STX);
	radio::write_txfifo(packet.raw, sizeof(packet.raw));
}

int main()
{
	PRR = _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRADC);

	sei();

	Main main;

	init(main);

	main.loop();
}

template uint8_t CC1101::CC1101<USI, Pin<Port<B>, 3>>::select();
template void CC1101::CC1101<USI, Pin<Port<B>, 3>>::write_txfifo(uint8_t* values, uint8_t len);
template void CC1101::CC1101<USI, Pin<Port<B>, 3>>::read_rxfifo(uint8_t* values, uint8_t len);
