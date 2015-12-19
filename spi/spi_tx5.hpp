#pragma once

#include <avr/io.h>
#include "port.hpp"

class SPI_tx5 {
public:
	static void setup() {
		MOSI::mode(OUTPUT);
		MISO::mode(INPUT);
		SCK::mode(OUTPUT);
		SCK::clear();
		USICR = _BV(USIWM0);
	}

	static unsigned char xmit(unsigned char);
private:
	typedef ::Port<B> Port;
	typedef Pin<Port, 0> MOSI;
	typedef Pin<Port, 1> MISO;
	typedef Pin<Port, 2> SCK;
};
