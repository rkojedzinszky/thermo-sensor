#include <stdint.h>
#include <util/crc16.h>
#include <common.hpp>

uint8_t crc8_ccitt(const uint8_t* buf, uint8_t len)
{
	uint8_t crc = 0;

	for (;len > 0; --len) {
		crc = _crc8_ccitt_update(crc, *buf);
		++buf;
	}

	return crc;
}
