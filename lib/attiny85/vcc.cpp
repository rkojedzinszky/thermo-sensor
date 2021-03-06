#include "vcc.hpp"

unsigned VCC::read_voltage()
{
	PRR &= ~_BV(PRTIM0) & ~_BV(PRADC);
	ADCSRA = _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0);
	ADMUX = _BV(MUX3) | _BV(MUX2);

	done_ = false;

	TIM0_COMPAInterrupt::set(interrupt);
	TCCR0A = 0;
	TCNT0 = 0;
	TCCR0B = _BV(CS01);
	OCR0A = 128;
	TIMSK |= _BV(OCIE0A);

	while (!done_) {
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
		TIM0_COMPAInterrupt::pending();
	}

	TIMSK &= ~_BV(OCIE0A);
	TCCR0B = 0;
	PRR |= _BV(PRTIM0);

	done_ = false;
	ADCInterrupt::set(interrupt);
	ADCSRA |= _BV(ADIE);
	while (!done_) {
		set_sleep_mode(SLEEP_MODE_ADC);
		sleep_mode();
		ADCInterrupt::pending();
	}
	ADCSRA = 0;

	PRR |= _BV(PRADC);

	TIM0_COMPAInterrupt::clear();
	ADCInterrupt::clear();

	return ADC;
}

void VCC::interrupt()
{
	done_ = true;
}

bool VCC::done_;
