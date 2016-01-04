#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <port.hpp>
#include <usi.hpp>
#include <cc1101.hpp>
#include <interrupt/PCINT0.hpp>
#include <sensorvalue.hpp>

typedef CC1101<USI, Pin<Port<A>, 7>> radio;

static void write_eeprom(int addr, unsigned char value)
{
	EECR = 0;

	EEAR = addr;
	EEDR = value;

	EECR |= _BV(EEMPE);
	EECR |= _BV(EEPE);

	loop_until_bit_is_clear(EECR, EEPE);
}

static unsigned int eeprom_addr = 0;

static void mark_eeprom_eof()
{
	write_eeprom(eeprom_addr, 0xff);
}

static void receive()
{
	while (radio::USI::DI::is_set()) {
		radio::select();
		unsigned char rxbytes = radio::status<radio::RXBYTES>();
		unsigned char d[rxbytes];
		radio::read_rxfifo(d, rxbytes);
		if (rxbytes == 34) {
			SensorValue<RSSI>::encode(static_cast<int>(d[32]) - 148, d + d[0]);
			d[0]++;
			for (int i = 0; i <= d[0]; ++i) {
				write_eeprom(eeprom_addr++, d[i]);
			}
		}
	}
	mark_eeprom_eof();
}

static void receive_loop()
{
	radio::select();
	radio::wcmd<radio::SRX>();
	radio::release();
	PCINT0Interrupt::set(receive);

	while(1) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		PCINT0Interrupt::pending();
	}
}

int main()
{
	mark_eeprom_eof();

	radio::setup();
	radio::reset();
	radio::select();
	radio::set<radio::IOCFG2>(0x7);
	radio::set<radio::IOCFG1>(0x7);
	radio::release();

	GIMSK |= _BV(PCIE0);
	PCMSK0 |= _BV(radio::USI::DI::pin);

	sei();

	receive_loop();
}
