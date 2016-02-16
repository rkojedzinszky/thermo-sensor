#pragma once

#include <avr/io.h>
#include <port.hpp>

class SPI {
public:
	static void setup() {
		SCK::mode(OUTPUT);
		DO::mode(OUTPUT);
		SCK::clear();
		DO::clear();
		SPCR = _BV(SPE) | _BV(MSTR);
	}

	static unsigned char xmit(unsigned char data) {
		SPDR = data;
		loop_until_bit_is_set(SPSR, SPIF);
		return SPDR;
	}
private:
	typedef ::Port<B> Port;
public:
	typedef Pin<Port, 5> SCK;
	typedef Pin<Port, 4> DI;
	typedef Pin<Port, 3> DO;
};
