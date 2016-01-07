#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <port.hpp>
#include <usi.hpp>
#include <radio.hpp>
#include <interrupt/PCINT0.hpp>
#include <sensorvalue.hpp>
#include <txuart.hpp>

typedef Radio<CC1101::CC1101<USI, Pin<Port<A>, 7>>> radio;
typedef TXUart<Pin<Port<B>, 0>, 1000000, 38400> txuart;

static void txbyte(unsigned char byte)
{
	static char nibble[] = "0123456789abcdef";
	txuart::txbyte(nibble[byte >> 4]);
	txuart::txbyte(nibble[byte & 15]);
}

static void txdata(unsigned char* data, unsigned char len)
{
	cli();
	for (;len > 0; --len) {
		txbyte(*data++);
	}
	txuart::txbyte('\n');
	sei();
}

static void receive()
{
	while (radio::USI::DI::is_set()) {
		radio::select();
		unsigned char rxbytes = radio::status<CC1101::RXBYTES>();
		unsigned char d[rxbytes];
		radio::read_rxfifo(d, rxbytes);
		if (rxbytes == 34) {
			SensorValue<RSSI>::encode(static_cast<int>(d[32]) - 148, d + d[0]);
			d[0] += 2;
			txdata(d, d[0]);
		}
	}
}

static void receive_loop()
{
	radio::select();
	radio::wcmd<CC1101::SRX>();
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
	txuart::Pin::mode(OUTPUT);
	txuart::Pin::set();

	radio::setup();
	radio::setup_for_rx();
	radio::select();
	radio::set<CC1101::IOCFG2>(0x7);
	radio::set<CC1101::IOCFG1>(0x7);
	radio::release();

	GIMSK |= _BV(PCIE0);
	PCMSK0 |= _BV(radio::USI::DI::pin);

	sei();

	receive_loop();
}
