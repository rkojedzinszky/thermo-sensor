#pragma once

#include <stdint.h>

class RFC2045 {
public:
	static uint8_t encode(const uint8_t* src, uint8_t slen, uint8_t* dst);
private:
	static uint8_t map(const uint8_t c);
};
