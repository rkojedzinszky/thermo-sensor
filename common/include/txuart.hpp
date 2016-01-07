#pragma once

#include <avr/io.h>

template <class Pin_t, long freq, long baud>
class TXUart {
	static constexpr int delay = freq / baud;

	static void start () {
		TCNT0 = delay / 3;
		OCR0A = delay;
		TCCR0A = _BV(WGM01);
		TCCR0B = _BV(CS00);
	}

	static void stop() {
		TCCR0B = 0;
	}

	static void wait() {
		loop_until_bit_is_set(TIFR0, OCF0A);
		TIFR0 |= _BV(OCF0A);
	}

public:
	typedef Pin_t Pin;

	static void txbyte(unsigned char byte) {
		start();
		Pin::clear();
		wait();
		for (char i = 0; i < 8; ++i) {
			if (byte & 1) {
				Pin::set();
			} else {
				Pin::clear();
			}
			byte >>= 1;
			wait();
		}
		Pin::set();
		wait();
		stop();
	}
};
