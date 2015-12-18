#include <avr/sleep.h>
#include "interrupt_impl.hpp"

template <>
void ADCInterrupt::enqueue(void (*f)())
{
	_enqueue(f);
	ADCSRA |= _BV(ADIE);
}

template <>
void ADCInterrupt::loop()
{
	_loop();
	ADCSRA &= ~_BV(ADIE);
}

template <>
void ADCInterrupt::sleep()
{
	set_sleep_mode(SLEEP_MODE_ADC);
	sleep_mode();

	if (bit_is_set(ADCSRA, ADIF)) {
		tick();
		ADCSRA &= ~_BV(ADIF);
	}
}

template class Interrupt<ADC_vect_num>;

ISR(ADC_vect)
{
	ADCInterrupt::tick();
}

template<>
Callback ADCInterrupt::queued_ = Callback();

template<>
Callback ADCInterrupt::pending_ = Callback();
