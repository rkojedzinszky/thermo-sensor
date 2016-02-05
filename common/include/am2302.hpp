#pragma once

#define F_CPU 1000000L
#include <util/delay.h>

#include "port.hpp"

template <class Pin>
class AM2302 {
public:
	static bool read(short& humidity, short& temperature);
private:
	static bool read8(unsigned char& value);
};

template <class Pin>
bool AM2302<Pin>::read(short& humidity, short& temperature)
{
	unsigned char d[5];

	Pin::mode(OUTPUT);
	Pin::clear();
	_delay_ms(1);
	Pin::set();
	_delay_us(30);
	Pin::mode(INPUT);
	Pin::set();
	if (!Pin::loop_until_set(85)) {
		return false;
	}
	if (!Pin::loop_until_clear(85)) {
		return false;
	}

	for (char i = 0; i < 5; ++i) {
		if (!read8(d[i])) {
			return false;
		}
	}

	if (!Pin::loop_until_set(55)) {
		return false;
	}

	humidity = (d[0] << 8) | d[1];
	temperature = ((d[2] & 0x7f) << 8) | d[3];
	if (d[2] & 0x80) {
		temperature = -temperature;
	}

	d[0] += d[1];
	d[0] += d[2];
	d[0] += d[3];

	return d[0] == d[4];
}

template <class Pin>
inline bool AM2302<Pin>::read8(unsigned char& value)
{
	for (char i = 0; i < 8; ++i) {
		value <<= 1;
		if (!Pin::loop_until_set(55)) {
			return false;
		}
		_delay_us(30);
		if (Pin::is_set()) {
			value |= 1;
		}
		if (!Pin::loop_until_clear(55)) {
			return false;
		}
	}

	return true;
}

