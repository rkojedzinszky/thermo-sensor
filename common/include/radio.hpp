#include <cc1101.hpp>

template <typename cc1101>
class Radio : public cc1101 {
	static void setup_common();
public:
	static void setup_for_rx();
	static void setup_for_tx();
};

template <typename cc1101>
inline void Radio<cc1101>::setup_common()
{
	// reset
	cc1101::select();
	cc1101::wcmd(CC1101::SRES);
	cc1101::release();

	cc1101::select();

	// disable GDO[0,2] pins
	cc1101::set(CC1101::IOCFG2, 0x2f);
	cc1101::set(CC1101::IOCFG0, 0x2f);

	// fix packet length
	cc1101::set(CC1101::PKTLEN, 16);

	// packet automation
	cc1101::set(CC1101::PKTCTRL0, 0x44);

	// frequency configuration
	cc1101::set(CC1101::FREQ2, 0x10);
	cc1101::set(CC1101::FREQ1, 0xa7);
	cc1101::set(CC1101::FREQ0, 0xe1);

	// modem configuration
	cc1101::set(CC1101::MDMCFG4, 0x8b);
	cc1101::set(CC1101::MDMCFG3, 0x83);
	cc1101::set(CC1101::MDMCFG2, 0x73);
	cc1101::set(CC1101::MDMCFG1, 0xa2);
	cc1101::set(CC1101::DEVIATN, 0x40);

	// calibrate
	cc1101::wcmd(CC1101::SCAL);
	while ((cc1101::status(CC1101::MARCSTATE) & 0x1f) != 1)
		;
}

template <typename cc1101>
inline void Radio<cc1101>::setup_for_rx()
{
	setup_common();

	// packet automation
	cc1101::set(CC1101::PKTCTRL1, 0x2c);

	// main radio control state machine configuration
	cc1101::set(CC1101::MCSM1, 0x3c);
	cc1101::set(CC1101::MCSM0, 0x34);

	cc1101::release();
}

template <typename cc1101>
inline void Radio<cc1101>::setup_for_tx()
{
	setup_common();

	// main radio control state machine configuration
	cc1101::set(CC1101::MCSM0, 0x38);

	// PATABLE
	cc1101::set(CC1101::PATABLE, 0xc0);

	cc1101::release();
}
