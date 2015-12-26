#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <common.hpp>
#include <port.hpp>

typedef Pin<Port<B>, 4> GDO0;

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

ISR(PCINT0_vect)
{
}

static void receive()
{
	if (GDO0::is_clear()) {
		return;
	}

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

static void receive_loop()
{
	radio::select();
	radio::wcmd<radio::SRX>();
	radio::release();

	while(1) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		receive();
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

	GIMSK |= _BV(PCIE);
	PCMSK |= _BV(PCINT4);

	sei();

	receive_loop();
}
