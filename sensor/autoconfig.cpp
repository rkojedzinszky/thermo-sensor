#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <interrupt/WDT.hpp>
#include <interrupt/PCINT0.hpp>
#include "sensor.hpp"
#include "autoconfig.hpp"
#include <packet.hpp>
#include <vcc.hpp>

static uint8_t gen_id()
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

	am2302::read(data.thum, data.ttemp);
	data.vcc = vccreader.read_voltage();

	return crc8_ccitt(data.raw, sizeof(data));
}

static void do_autoconfig(Config& config)
{
	ConfigRequestPacket req(config.id());

	WDTCR = _BV(WDIE) | _BV(WDP2) | _BV(WDP1) | _BV(WDP0); // 2 secs
	PCMSK |= _BV(radio::USI::DI::pin);

	for (;;) {
		radio::select();
		radio::wcmd(CC1101::SIDLE);
		radio::wcmd(CC1101::SFRX);
		radio::wcmd(CC1101::SFTX);
		radio::wcmd(CC1101::STX);
		radio::write_txfifo(&req.len_, req.len_ + 1);

		GIMSK |= _BV(PCIE);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		GIMSK &= ~_BV(PCIE);

		if (radio::USI::DI::is_set()) {
			radio::select();
			uint8_t rxbytes = radio::status(CC1101::RXBYTES);
			if (rxbytes == sizeof(ConfigResponsePacket)) {
				ConfigResponsePacket resp;
				radio::read_rxfifo(&resp.len_, rxbytes);

				config = resp.config_;
				config.write();
				break;
			}
		}
	}

	radio::select();
	radio::wcmd(CC1101::SIDLE);
	radio::release();

	WDTCR = 0;

	WDTInterrupt::fire_ = false;
	PCINT0Interrupt::fire_ = false;
}

void autoconfig(Config& config)
{
	sei();

	config.id() = gen_id();

	radio::select();
	radio::set(CC1101::PKTCTRL1, 0x09);
	radio::set(CC1101::PKTCTRL0, 0x45);
	radio::set(CC1101::IOCFG1, 0x07);
	radio::set(CC1101::MCSM1, 0x03);
	radio::set(CC1101::ADDR, config.id());
	radio::release();

	do_autoconfig(config);

	cli();
}

