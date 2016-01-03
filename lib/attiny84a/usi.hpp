#pragma once

#include <avr/io.h>
#include <port.hpp>

class USI {
public:
	static void setup() {
		DI::mode(INPUT);
		DO::mode(OUTPUT);
		USCK::mode(OUTPUT);
		USCK::clear();
	}

	static unsigned char xmit(unsigned char);
private:
	typedef ::Port<A> Port;
public:
	typedef Pin<Port, 6> DI;
	typedef Pin<Port, 5> DO;
	typedef Pin<Port, 4> USCK;
};
