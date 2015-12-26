#pragma once

#include <avr/io.h>
#include "port.hpp"

class SPI_tx5 {
public:
	static void setup() {
		DI::mode(INPUT);
		DO::mode(OUTPUT);
		USCK::mode(OUTPUT);
		USCK::clear();
	}

	static unsigned char xmit(unsigned char);
private:
	typedef ::Port<B> Port;
public:
	typedef Pin<Port, 0> DI;
	typedef Pin<Port, 1> DO;
	typedef Pin<Port, 2> USCK;
};