#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <port.hpp>
#include <usi.hpp>
#include <cc1101.hpp>
#include <interrupt/WATCHDOG.hpp>
#include <interrupt/PCINT0.hpp>
#include <sensorvalue.hpp>
#include <vcc.hpp>
#include <am2302.hpp>

typedef AM2302< Pin<Port<B>, 1> > Tsensor1;
typedef Pin<Port<B>, 0> LED;
typedef CC1101<USI, Pin<Port<A>, 7>> radio;

unsigned char id;

static unsigned char read_eeprom(int addr)
{
	loop_until_bit_is_clear(EECR, EEPE);

	EEAR = addr;

	EECR |= _BV(EERE);

	return EEDR;
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
	radio::wcmd<radio::STX>();
	radio::write_txfifo(data.raw, data.len + 1);
}

static void loop();

static void radio_off()
{
	GIMSK &= ~_BV(PCIE0);

	radio::select();
	radio::wcmd<radio::SPWD>();
	radio::release();

	wdt_reset();
	WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0);
	WATCHDOGInterrupt::set(loop);

	LED::clear();
}

#define P_TIMEOUTS 3

static void loop()
{
	static unsigned char counter = 0;

	counter++;

	if (counter == P_TIMEOUTS) {
		short thum, ttemp;
		Tsensor1::read(thum, ttemp);
		WDTCSR = _BV(WDIE) | _BV(WDP3);
	} else if (counter == P_TIMEOUTS+1) {
		LED::set();
		counter = 0;
		send();
		WDTCSR = _BV(WDP2) | _BV(WDP0) | _BV(WDIE);
		WATCHDOGInterrupt::set(radio_off);

		GIMSK |= _BV(PCIE0);

		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		PCINT0Interrupt::pending();
	}
}

int main()
{
	id = read_eeprom(511);

	radio::setup();
	radio::reset();
	radio::select();
	radio::set<radio::IOCFG1>(0x06);
	radio::release();

	LED::mode(OUTPUT);

	sei();

	PCMSK0 |= _BV(radio::USI::DI::pin);
	PCINT0Interrupt::set(radio_off);

	radio_off();

	for (;;) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		WATCHDOGInterrupt::pending();
	}
}
