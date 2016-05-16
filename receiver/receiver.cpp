#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

extern "C" {
#include <aes/aes.h>
};

#include <port.hpp>
#include <radio.hpp>
#include <interrupt/PCINT0.hpp>
#include <sensorvalue.hpp>
#include <common.hpp>
#include <packet.hpp>
#include "receiver.hpp"
#include "autoconfig.hpp"
#include "rfc2045.h"

static unsigned short magic;
static aes128_ctx_t aes_ctx;
static void txdata(unsigned char* data, unsigned char len);

void init()
{
	Config config;
	config.read();

	radio::setup();
	radio::setup_basic();

	autoconfig(config);

	radio::select();

	for (auto c = config.radioconfig(); c->reg_ != 0xff; ++c) {
		radio::set(c->reg(), c->value);
	}

	radio::setup_for_rx();

	radio::set(CC1101::IOCFG2, 0x7);
	radio::set(CC1101::IOCFG1, 0x7);

	radio::release();

	magic = config.magic();
	aes128_init(config.key(), &aes_ctx);
}

static void txchr(unsigned char chr)
{
	while (!usart::tx(chr)) {
	}
}

static void txdata(unsigned char* data, unsigned char len)
{
	uint8_t b64[32];

	uint8_t l = RFC2045::encode(data, len, b64);

	for (uint8_t i = 0; i < l; ++i) {
		txchr(b64[i]);
	}

	txchr('\r');
	txchr('\n');
}

static void receive()
{
	while (radio::USI::DI::is_set()) {
		radio::select();
		unsigned char rxbytes = radio::status(CC1101::RXBYTES);
		if (rxbytes == sizeof(Radiopacket_status)) {
			Radiopacket_status packet;

			radio::read_rxfifo(packet.raw, sizeof(Radiopacket_status));

			aes128_dec(packet.raw, &aes_ctx);

			if (packet.magic != magic) {
				return;
			}

			unsigned char* s = packet.raw + 2;
			unsigned char* e = packet.raw + packet.len;
			char rssi = packet.rssi;
			unsigned char lqi = packet.lqi;
			e += SensorValue::RSSI::encode(static_cast<int>(rssi), e);
			e += SensorValue::LQI::encode(static_cast<int>(lqi & 0x7f), e);

			packet.len = e - s;

			txdata(s, packet.len);
		} else {
			unsigned char d[rxbytes];
			radio::read_rxfifo(d, rxbytes);
		}
	}
}

static void receive_loop()
{
	radio::select();
	radio::wcmd(CC1101::SRX);
	radio::release();
	PCINT0Interrupt::set(receive);

	set_sleep_mode(SLEEP_MODE_IDLE);

	while(1) {
		cli();
		if (PCINT0Interrupt::fire_ == false) {
			sleep_enable();
			sei();
			sleep_cpu();
			sleep_disable();
		} else {
			sei();
			PCINT0Interrupt::pending();
		}
	}
}

/*{{{ uart */
ISR(USART_RX_vect)
{
	usart::rx_ready();
}

ISR(USART_UDRE_vect)
{
	usart::tx_ready();
}/*}}}*/

int main()
{
	init();

	usart::init();

	PCICR |= _BV(PCIE0);
	PCMSK0 |= _BV(radio::USI::DI::pin);

	sei();

	receive_loop();
}

template<>
usart::rxfifo_t usart::rxfifo = usart::rxfifo_t();
template<>
usart::txfifo_t usart::txfifo = usart::txfifo_t();
