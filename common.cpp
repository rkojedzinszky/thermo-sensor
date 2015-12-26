#include <common.hpp>

void radio_reset()
{
	// reset
	radio::select();
	radio::wcmd<radio::SRES>();
	radio::release();

	radio::select();

	// 433MHz center freq
	radio::set<radio::FREQ2>(0x10);
	radio::set<radio::FREQ1>(0xa7);
	radio::set<radio::FREQ0>(0x63);

	// modulation
	radio::set<radio::MDMCFG2>(0x1f);

	// calibration
	radio::set<radio::FSCAL3>(0xea);
	radio::set<radio::FSCAL2>(0x2a);
	radio::set<radio::FSCAL1>(0x00);
	radio::set<radio::FSCAL0>(0x1f);

	radio::set<radio::TEST2>(0x81);
	radio::set<radio::TEST1>(0x35);
	radio::set<radio::TEST0>(0x09);

	radio::set<radio::PKTCTRL1>(0x28);

	radio::set<radio::PATABLE>(0x60);

	radio::wcmd<radio::SCAL>();

	radio::set<radio::MCSM0>(0x34);

	radio::set<radio::PKTLEN>(32);

	radio::release();
}
