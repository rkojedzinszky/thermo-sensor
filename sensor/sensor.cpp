#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

extern "C" {
#include <aes/aes.h>
};

#include <port.hpp>
#include <usi.hpp>
#include <radio.hpp>
#include <interrupt/WDT.hpp>
#include <interrupt/PCINT0.hpp>
#include <sensorvalue.hpp>
#include <vcc.hpp>
#include <am2302.hpp>
#include <common.hpp>
#include <config.hpp>

typedef AM2302< Pin<Port<B>, 4> > Tsensor1;
typedef Radio<CC1101::CC1101<USI, Pin<Port<B>, 3>>> radio;

static constexpr int wdt_p_timeouts = 3;
static unsigned short magic;
static unsigned char id;
static aes128_ctx_t aes_ctx;

static void config_init(Config& config)
{
	VCC vccreader;
	union {
		struct {
			short thum, ttemp;
			short vcc;
			uint8_t timer;
		};
		uint8_t raw[1];
	} data;

	TCCR0B |= _BV(CS00);
	WDTCR = _BV(WDIE) | _BV(WDP3); // 4 secs
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
	WDTInterrupt::fire_ = false;
	WDTCR = 0;
	data.timer = TCNT0;
	TCCR0B = 0;

	Tsensor1::read(data.thum, data.ttemp);
	data.vcc = vccreader.read_voltage();

	config.id() = crc8_ccitt(data.raw, sizeof(data));
}

void init()
{
	Config config;

	config.read();

	if (!config.valid()) {
		config_init(config);
	}

	magic = config.magic;
	id = config.id;

	aes128_init(config.key, &aes_ctx);
}


static void send()
{
	static unsigned short seq_ = 0;
	Radiopacket packet;

	VCC vccreader;
	unsigned char* dp = packet.data;

	short hum, temp;
	if (Tsensor1::read(hum, temp)) {
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

static void radio_off()
{
	GIMSK &= ~_BV(PCIE);

	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();

	WDTCR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0); // 8 secs
	WDTInterrupt::set(loop);
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
		WDTCR = _BV(WDIE) | _BV(WDP3); // 4 secs
		short thum, ttemp;
		Tsensor1::read(thum, ttemp);
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

	radio::setup();
	radio::setup_for_tx();
	radio::select();
	radio::set(CC1101::IOCFG1, 0x06);
	radio::set(CC1101::IOCFG0, 0x06);
	radio::release();

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
