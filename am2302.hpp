#pragma once

#define F_CPU 1000000L
#include <util/delay.h>

#include "port.hpp"

template <class Pin>
class AM2302 {
public:
	static bool read(short& humidity, short& temperature);
private:
	static unsigned char read8();
};

template <class Pin>
inline bool AM2302<Pin>::read(short& humidity, short& temperature)
{
	unsigned char d[5];

	Pin::mode(OUTPUT);
	Pin::clear();
	_delay_ms(1);
	Pin::set();
	_delay_us(30);
	Pin::clear();
	Pin::mode(INPUT);
	Pin::set(); // pull-up
	_delay_us(40);
	Pin::loop_until_set();
	Pin::loop_until_clear();

	for (char i = 0; i < 5; ++i) {
		d[i] = read8();
	}

	Pin::loop_until_set();

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
inline unsigned char AM2302<Pin>::read8()
{
	unsigned char r;

	for (char i = 0; i < 8; ++i) {
		r <<= 1;
		Pin::loop_until_set();
		_delay_us(30);
		if (Pin::is_set()) {
			r |= 1;
		}
		Pin::loop_until_clear();
	}

	return r;
}

