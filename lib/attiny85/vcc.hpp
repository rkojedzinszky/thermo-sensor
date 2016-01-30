#pragma once

#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/ADC.hpp>

class VCC {
public:
	VCC() {
		PRR &= ~_BV(PRADC);
		ADCSRA = _BV(ADEN) | _BV(ADPS2);
		ADMUX = _BV(MUX3) | _BV(MUX2);

	}

	~VCC() {
		ADCSRA = 0;
		PRR |= _BV(PRADC);
	}

	unsigned read_voltage() {
		done_ = false;
		ADCInterrupt::set(interrupt);
		ADCSRA |= _BV(ADIE);
		while (!done_) {
			set_sleep_mode(SLEEP_MODE_ADC);
			sleep_mode();
			ADCInterrupt::pending();
		}
		ADCSRA &= ~_BV(ADIE);
		ADCInterrupt::clear();

		return ADC;
	}
private:
	static void interrupt();
	static bool done_;
};
