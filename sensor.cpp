#include <avr/io.h>
#define F_CPU 1000000L
#include <util/delay.h>
#include "ev/interrupt.hpp"
#include <common.hpp>
#include <am2302.hpp>

typedef AM2302< Pin<Port<B>, 4> > Tsensor1;

unsigned char id;

static unsigned char read_eeprom(int addr)
{
	loop_until_bit_is_clear(EECR, EEPE);

	EEAR = addr;

	EECR |= _BV(EERE);

	return EEDR;
}

static void send()
{
	union {
		struct {
			unsigned char len;
			unsigned char id_;
			short humidity, temperature;
		} data;
		unsigned char raw[0];
	} data;

	if (!Tsensor1::read(data.data.humidity, data.data.temperature)) {
		return;
	}

	data.data.id_ = id;
	data.data.len = sizeof(data) - 1;

	radio::select();
	radio::write_txfifo(data.raw, sizeof(data));
	radio::select();
	radio::wcmd<radio::STX>();
	radio::release();
}

static void loop();

static void radio_off()
{
	radio::select();
	radio::wcmd<radio::SPWD>();
	radio::release();

	WDTCR = _BV(WDP3) | _BV(WDP0);
	WDTInterrupt::enqueue(loop);
}

static void loop()
{
	static unsigned char counter = 0;

	counter++;

	if (counter == 3) {
		short thum, ttemp;
		Tsensor1::read(thum, ttemp);

		WDTCR = _BV(WDP3);
		WDTInterrupt::enqueue(loop);
	} else if (counter == 4) {
		counter = 0;
		send();
		WDTCR = _BV(WDP1);
		WDTInterrupt::enqueue(radio_off);
	} else {
		WDTInterrupt::enqueue(loop);
	}
}

int main()
{
	id = read_eeprom(511);

	radio::setup();
	radio_reset();

	sei();

	radio_off();
	WDTInterrupt::loop();
}
