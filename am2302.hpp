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
bool AM2302<Pin>::read(short& humidity, short& temperature)
{
	unsigned char d[5];

	Pin::mode(OUTPUT);
	Pin::low();

	_delay_ms(1);

	Pin::high();
	_delay_us(30);

	Pin::low();

	Pin::mode(INPUT);
	Pin::high(); // pull-up

	_delay_us(40);

	while (Pin::value() == 0) { }

	while (Pin::value() != 0) { }

	for (char i = 0; i < 5; ++i) {
		d[i] = read8();
	}

	while (Pin::value() == 0) { }

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
unsigned char AM2302<Pin>::read8()
{
	unsigned char r;

	for (char i = 0; i < 8; ++i) {
		r <<= 1;
		while (Pin::value() == 0) { }
		_delay_us(30);
		if (Pin::value() != 0) {
			r |= 1;
		}
		while (Pin::value() != 0) { }
	}

	return r;
}

