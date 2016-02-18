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

	TCCR0B = _BV(CS00);

	thermo_on(true);
	WDTCR = _BV(WDIE) | _BV(WDP3); // 4 secs
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
	WDTCR = 0;
	data.timer = TCNT0;
	TCCR0B = 0;

	am2302::read(data.thum, data.ttemp);
	thermo_on(false);
	data.vcc = vccreader.read_voltage();

	WDTInterrupt::fire_ = false;

	return crc8_ccitt(data.raw, sizeof(data));
}

static void do_send(ConfigRequestPacket& packet)
{
	radio::select();
	radio::wcmd(CC1101::STX);
	radio::write_txfifo(&packet.len_, packet.len_ + 1);
}

static bool do_receive(Config& config)
{
	radio::select();
	uint8_t rxbytes = radio::status(CC1101::RXBYTES);
	if (rxbytes == sizeof(ConfigResponsePacket)) {
		ConfigResponsePacket resp;

		radio::read_rxfifo(&resp.len_, rxbytes);

		config = resp.config_;
		return true;
	}

	radio::sidle();
	radio::wcmd(CC1101::SFRX);
	radio::wcmd(CC1101::SRX);
	radio::release();

	return false;
}

static bool receive_with_timeout(Config& config)
{
	WDTCR = _BV(WDIE) | _BV(WDP1) | _BV(WDP0); // 0.125s

	// wait for packet with timeout
	WDTInterrupt::fire_ = false;
	while (WDTInterrupt::fire_ == false) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();

		if (radio::USI::DI::is_set()) {
			if (do_receive(config)) {
				return true;
			}
		}
	}

	return false;
}

static void radio_off()
{
	radio::select();
	radio::sidle();
	radio::wcmd(CC1101::SPWD);
	radio::release();
}

static void do_autoconfig(Config& config)
{
	ConfigRequestPacket req(config.id());

	PCMSK = _BV(radio::USI::DI::pin);
	GIMSK |= _BV(PCIE);

	for (;;) {
		do_send(req);

		if (receive_with_timeout(config)) {
			break;
		}

		radio_off();

		WDTCR = _BV(WDIE) | _BV(WDP3); // 4 secs
		WDTInterrupt::fire_ = false;

		while (WDTInterrupt::fire_ == false) {
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
		}
	}

	GIMSK &= ~_BV(PCIE);
	WDTCR = 0;

	radio::select();
	radio::sidle();
	radio::release();

	WDTInterrupt::fire_ = false;
	PCINT0Interrupt::fire_ = false;
}

void autoconfig(Config& config)
{
	if (config.id() == 0xff) {
		config.id() = gen_id() | 0x80;
	}

	radio::select();
	radio::set(CC1101::PKTCTRL1, 0x09);
	radio::set(CC1101::PKTCTRL0, 0x45);
	radio::set(CC1101::IOCFG1, 0x07);
	radio::set(CC1101::IOCFG0, 0x06);
	radio::set(CC1101::MCSM1, 0x0f);
	radio::set(CC1101::ADDR, config.id());
	radio::release();

	do_autoconfig(config);

	config.write();
}

