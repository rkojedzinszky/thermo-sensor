#include <avr/sleep.h>
#include "interrupt_impl.hpp"

template <>
void WDTInterrupt::enqueue(void (*f)())
{
	_enqueue(f);
	WDTCR |= _BV(WDIE);
}

template <>
void WDTInterrupt::loop()
{
	_loop();
	WDTCR &= ~_BV(WDIE);
}

template <>
void WDTInterrupt::sleep()
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();

	if (bit_is_set(WDTCR, WDIF)) {
		tick();
		WDTCR &= ~_BV(WDIF);
	}
}

template class Interrupt<WDT_vect_num>;

ISR(WDT_vect)
{
	WDTInterrupt::tick();
}

template<>
Callback WDTInterrupt::queued_ = Callback();

template<>
Callback WDTInterrupt::pending_ = Callback();
