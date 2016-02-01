#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <interrupt/WATCHDOG.hpp>
#include <interrupt/PCINT0.hpp>
#include "receiver.hpp"
#include "autoconfig.hpp"
#include <packet.hpp>
#include <vcc.hpp>

static uint8_t autoconfig_timeout = 4;

static void autoconfig_wdt()
{
	--autoconfig_timeout;
}

static void do_autoconfig(Config& config)
{
	ConfigRequestPacket req;

	WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0); // 8 secs
	WATCHDOGInterrupt::set(autoconfig_wdt);
	PCMSK0 |= _BV(radio::USI::DI::pin);

	GIMSK |= _BV(PCIE0);

	for (;;) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		WATCHDOGInterrupt::pending();
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
				radio::select();
				radio::wcmd(CC1101::SIDLE);
				radio::wcmd(CC1101::STX);
				radio::write_txfifo(&resp.len_, resp.len_ + 1);

				config.id() += 1;
				if (config.id() == 0xff) {
					config.id() = 1;
				}
				config.write();
			} else {
				radio::wcmd(CC1101::SIDLE);
				radio::wcmd(CC1101::SFRX);
				radio::wcmd(CC1101::SRX);
				radio::release();
			}
		}
	}

	GIMSK &= ~_BV(PCIE0);

	radio::select();
	radio::wcmd(CC1101::SIDLE);
	radio::release();

	WDTCSR = 0;

	WATCHDOGInterrupt::fire_ = false;
	PCINT0Interrupt::fire_ = false;
}

void autoconfig(Config& config)
{
	sei();

	radio::select();
	radio::set(CC1101::PKTCTRL1, 0x0a);
	radio::set(CC1101::PKTCTRL0, 0x45);
	radio::set(CC1101::IOCFG1, 0x07);
	radio::set(CC1101::MCSM1, 0x0f);
	radio::set(CC1101::ADDR, 0);
	radio::wcmd(CC1101::SRX);
	radio::release();

	do_autoconfig(config);

	cli();
}
