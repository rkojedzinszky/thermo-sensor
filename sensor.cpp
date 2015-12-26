#include <avr/io.h>
#define F_CPU 1000000L
#include <util/delay.h>
#include "ev/interrupt.hpp"
#include <common.hpp>
#include <am2302.hpp>
#include <sensor.hpp>
#include <vcc.hpp>

typedef AM2302< Pin<Port<B>, 4> > Tsensor1;

unsigned char id;

static unsigned char read_eeprom(int addr)
{
	loop_until_bit_is_clear(EECR, EEPE);

	EEAR = addr;

	EECR |= _BV(EERE);

	return EEDR;
}

ISR(ADC_vect)
{
}

void send()
{
	union {
		struct {
			unsigned char len;
			unsigned char id;
			unsigned char data[0];
		};
		unsigned char raw[16];
	} data;

	VCC vccreader;
	unsigned char* dp = data.data;

	short hum, temp;
	if (Tsensor1::read(hum, temp)) {
		dp += SensorValue<Humidity>::encode(hum, dp);
		dp += SensorValue<Temperature>::encode(temp, dp);
	}

	int voltage = vccreader.read_voltage();
	dp += SensorValue<Power>::encode(voltage, dp);

	data.len = dp - data.data + 1;
	data.id = id;

	radio::select();
	radio::write_txfifo(data.raw, data.len + 1);
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
		WDTCR = 0;
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
