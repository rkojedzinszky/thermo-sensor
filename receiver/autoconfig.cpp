#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <interrupt/WDT.hpp>
#include <interrupt/PCINT0.hpp>
#include "receiver.hpp"
#include "autoconfig.hpp"
#include <packet.hpp>

static uint8_t autoconfig_timeout = 2;

static void autoconfig_wdt()
{
	--autoconfig_timeout;
}

static void do_autoconfig(Config& config)
{
	bool update_config = false;

	WDTInterrupt::set(autoconfig_wdt);
	wdt_reset();
	WDTCSR = _BV(WDCE) | _BV(WDE);
	WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0); // 8 secs

	PCMSK0 |= _BV(radio::USI::DI::pin);

	for (;;) {
		PCICR |= _BV(PCIE0);
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		PCICR &= ~_BV(PCIE0);

		WDTInterrupt::pending();
		if (autoconfig_timeout == 0) {
			break;
		}

		if (radio::USI::DI::is_set()) {
			radio::select();
			uint8_t rxbytes = radio::status(CC1101::RXBYTES);
			if (rxbytes == sizeof(ConfigRequestPacket)) {
				ConfigRequestPacket req;
				radio::read_rxfifo(&req.len_, rxbytes);

				ConfigResponsePacket resp(req.src_, config);
				if (req.src_ & 0x80) {
					config.id() += 1;
					if (config.id() == 0x80) {
						config.id() = 1;
					}
					update_config = true;
				} else {
					resp.config_.id() = req.src_;
				}

				radio::select();
				radio::wcmd(CC1101::SIDLE);
				radio::wcmd(CC1101::STX);
				radio::write_txfifo(&resp.len_, resp.len_ + 1);
			} else {
				radio::wcmd(CC1101::SIDLE);
				while ((radio::status(CC1101::MARCSTATE) & 0x1f) != 1)
					;
				radio::wcmd(CC1101::SFRX);
				radio::wcmd(CC1101::SRX);
				radio::release();
			}
		}
	}

	wdt_reset();
	WDTCSR = _BV(WDCE) | _BV(WDE);
	WDTCSR = 0;

	radio::select();
	radio::wcmd(CC1101::SIDLE);
	radio::release();

	WDTInterrupt::fire_ = false;
	PCINT0Interrupt::fire_ = false;

	if (update_config) {
		config.write();
	}
}

void autoconfig(Config& config)
{
	sei();

	radio::select();
	radio::set(CC1101::PKTCTRL1, 0x0a);
	radio::set(CC1101::PKTCTRL0, 0x45);
	radio::set(CC1101::IOCFG2, 0x6f);
	radio::set(CC1101::IOCFG1, 0x07);
	radio::set(CC1101::IOCFG0, 0x07);
	radio::set(CC1101::MCSM1, 0x0f);
	radio::set(CC1101::ADDR, 0);
	radio::wcmd(CC1101::SRX);
	radio::release();

	do_autoconfig(config);

	cli();
}

