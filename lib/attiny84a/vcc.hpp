#pragma once

#include <avr/io.h>
#include <avr/sleep.h>
#include <interrupt/ADC.hpp>

class VCC {
public:
	VCC() {
		PRR &= ~_BV(PRADC);
		ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
		ADMUX = _BV(MUX5) | _BV(MUX0);

	}

	~VCC() {
		ADCSRA = 0;
		PRR |= _BV(PRADC);
	}

	int read_voltage() {
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

		if (ADC > 0) {
			return (1023L * 1100L / ADC);
		}

		return -1;
	}
private:
	static void interrupt();
	static bool done_;
};
