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

typedef AM2302< Pin<Port<B>, 4> > Tsensor1;
typedef Radio<CC1101::CC1101<USI, Pin<Port<B>, 3>>> radio;

static unsigned short magic;
static unsigned char id;
static aes128_ctx_t aes_ctx;

void init()
{
	Deviceconfig config;

	config.read();

	magic = config.magic;
	id = config.id;

	aes128_init(config.key, &aes_ctx);
}

void send()
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
	radio::wcmd<CC1101::STX>();
	radio::write_txfifo(packet.raw, sizeof(packet.raw));
}

static void loop();

static void radio_off()
{
	GIMSK &= ~_BV(PCIE);

	radio::select();
	radio::wcmd<CC1101::SPWD>();
	radio::release();

	wdt_reset();
	WDTCR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0);
	WDTInterrupt::set(loop);
}

static void pcint()
{
	if (radio::USI::DI::is_clear()) {
		radio_off();
	}
}

#define P_TIMEOUTS 3

static void loop()
{
	static unsigned char counter = 0;

	counter++;

	if (counter == P_TIMEOUTS) {
		short thum, ttemp;
		Tsensor1::read(thum, ttemp);
		WDTCR = _BV(WDIE) | _BV(WDP3);
	} else if (counter == P_TIMEOUTS+1) {
		counter = 0;
		send();

		WDTCR = _BV(WDP2) | _BV(WDP0) | _BV(WDIE);
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
	radio::set<CC1101::IOCFG1>(0x06);
	radio::set<CC1101::IOCFG0>(0x06);
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
