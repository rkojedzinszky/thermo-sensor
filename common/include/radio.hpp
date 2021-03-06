#pragma once

#include <cc1101.hpp>

template <typename cc1101>
class Radio : public cc1101 {
public:
	static void calibrate();
	static void setup_basic();
	static void setup_common();
	static void setup_for_rx();
	static void setup_for_tx();

	static void sidle();
};

template <typename cc1101>
inline void Radio<cc1101>::calibrate()
{
	// calibrate
	cc1101::wcmd(CC1101::SCAL);
	while ((cc1101::status(CC1101::MARCSTATE) & 0x1f) != 1)
		;
}

template <typename cc1101>
inline void Radio<cc1101>::setup_basic()
{
	// reset
	cc1101::select();
	cc1101::wcmd(CC1101::SRES);
	cc1101::release();

	cc1101::select();

	// frequency configuration
	cc1101::set(CC1101::FREQ2, 0x10);
	cc1101::set(CC1101::FREQ1, 0xa7);
	cc1101::set(CC1101::FREQ0, 0xe1);

	// modem configuration
	cc1101::set(CC1101::MDMCFG4, 0xaa);
	cc1101::set(CC1101::MDMCFG3, 0xbc);
	cc1101::set(CC1101::MDMCFG2, 0x71);
	cc1101::set(CC1101::DEVIATN, 0x40);

	// PATABLE
	cc1101::set(CC1101::PATABLE, 0xc0);

	// enable WOR timer
	cc1101::set(CC1101::WORCTRL, 0x7a);

	calibrate();

	cc1101::release();
}

template <typename cc1101>
inline void Radio<cc1101>::setup_common()
{
	// disable GDO[0,2] pins
	cc1101::set(CC1101::IOCFG2, 0x2e);
	cc1101::set(CC1101::IOCFG0, 0x2e);

	// fix packet length
	cc1101::set(CC1101::PKTLEN, 16);

	// packet automation
	cc1101::set(CC1101::PKTCTRL0, 0x44);
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

	calibrate();
}

template <typename cc1101>
inline void Radio<cc1101>::setup_for_tx()
{
	setup_common();

	// main radio control state machine configuration
	cc1101::set(CC1101::MCSM1, 0x30);
	cc1101::set(CC1101::MCSM0, 0x38);

	calibrate();
}

template <typename cc1101>
inline void Radio<cc1101>::sidle()
{
	cc1101::wcmd(CC1101::SIDLE);
	while ((cc1101::status(CC1101::MARCSTATE) & 0x1f) != 1)
		;
}
