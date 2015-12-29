#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <interrupt/PCINT0.hpp>
#include <common.hpp>
#include <port.hpp>

typedef Pin<Port<A>, 3> GDO0;

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

static void receive()
{
	while (GDO0::is_set()) {
		radio::select();
		unsigned char rxbytes = radio::status<radio::RXBYTES>();
		unsigned char d[rxbytes];
		radio::read_rxfifo(d, rxbytes);
		if (rxbytes <= 32) {
			for (int i = 0; i < rxbytes; ++i) {
				write_eeprom(eeprom_addr++, d[i]);
			}
		}
	}
	write_eeprom(eeprom_addr, 0xff);
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
	GDO0::mode(INPUT);

	radio::setup();
	radio_reset();
	radio::select();
	radio::set<radio::IOCFG2>(0x7);
	radio::set<radio::IOCFG0>(0x7);
	radio::release();

	GIMSK |= _BV(PCIE0);
	PCMSK0 |= _BV(PCINT3);

	sei();

	receive_loop();
}
