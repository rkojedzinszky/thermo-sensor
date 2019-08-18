#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <interrupt/WDT.hpp>
#include <interrupt/PCINT0.hpp>
#include "sensor.hpp"
#include "autoconfig.hpp"
#include <packet.hpp>
#include <vcc.hpp>

static constexpr int delay = 10;

static constexpr unsigned int delay_ticks = delay * delay_multiplier;

static uint8_t gen_id()
{
	VCC vccreader;
	union {
		struct {
			uint16_t thum, ttemp;
			short vcc;
			uint8_t timer;
		};
		uint8_t raw[1];
	} data;

	TCCR0B = _BV(CS00);

	wdt_reset();
	WDTCR = _BV(WDIE) | _BV(WDP0); // 32 msecs
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
	WDTCR = 0;
	data.timer = TCNT0;
	TCCR0B = 0;

	USICR = 0;
	htu21d->read_temp(data.ttemp);
	htu21d->read_hum(data.thum);
	data.vcc = vccreader.read_voltage();

	WDTInterrupt::fire_ = false;

	return crc8_dallas(0, data.raw, sizeof(data));
}

static void do_send(ConfigRequestPacket& packet)
{
	radio::select();
	radio::set(CC1101::IOCFG1, 0x07);
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

	radio::wcmd(CC1101::SFRX);
	radio::release();

	return false;
}

static bool receive_with_timeout(Config& config)
{
	wdt_reset();
	WDTCR = _BV(WDIE) | _BV(WDP0); // 32msec

	// wait for packet with timeout
	PCMSK = _BV(radio::USI::DI::pin);
	GIMSK |= _BV(PCIE);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	GIMSK &= ~_BV(PCIE);
	WDTCR = 0;

	if (radio::USI::DI::is_set()) {
		return do_receive(config);
	}

	return false;
}

static void radio_off()
{
	radio::select();
	radio::wcmd(CC1101::SPWD);
	radio::release();
}

static void do_autoconfig(Config& config)
{
	ConfigRequestPacket req(config.id());

	for (;;) {
		do_send(req);

		if (receive_with_timeout(config)) {
			break;
		}

		radio::select();
		radio::set(CC1101::IOCFG1, 0xa4);
		radio::wcmd(CC1101::SPWD);
		radio::release();

		PCMSK = _BV(radio::USI::DI::pin);
		GIMSK |= _BV(PCIE);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		GIMSK &= ~_BV(PCIE);
	}

	GIMSK &= ~_BV(PCIE);
	WDTCR = 0;

	radio_off();

	WDTInterrupt::fire_ = false;
	PCINT0Interrupt::fire_ = false;
}

void autoconfig(Config& config)
{
	if (config.id() == 0xff) {
		config.id() = gen_id() | 0x80;
	}

	radio::select();
	radio::set(CC1101::WOREVT1, delay_ticks >> 8);
	radio::set(CC1101::WOREVT0, delay_ticks & 0xff);
	radio::set(CC1101::PKTCTRL1, 0x09);
	radio::set(CC1101::PKTCTRL0, 0x45);
	radio::set(CC1101::IOCFG0, 0x06);
	radio::set(CC1101::MCSM2, 0x01);
	radio::set(CC1101::MCSM1, 0x03);
	radio::set(CC1101::ADDR, config.id());
	radio::release();

	do_autoconfig(config);

	config.write();
}

