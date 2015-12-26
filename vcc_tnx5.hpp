#pragma once

#include <avr/io.h>
#include <avr/sleep.h>

class VCC_tnx5 {
public:
	VCC_tnx5() {
		PRR &= ~_BV(PRADC);
		ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
		ADMUX = _BV(MUX3) | _BV(MUX2);

	}

	~VCC_tnx5() {
		ADCSRA = 0;
		PRR |= _BV(PRADC);
	}

	int read_voltage() {
		ADCSRA |= _BV(ADIE);
		set_sleep_mode(SLEEP_MODE_ADC);
		sleep_mode();
		loop_until_bit_is_clear(ADCSRA, ADSC);
		ADCSRA &= ~_BV(ADIE);

		if (ADC > 0) {
			return (1023L * 1100L / ADC);
		}

		return -1;
	}
};
