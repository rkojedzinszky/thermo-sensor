#include <avr/sleep.h>
#include "interrupt_impl.hpp"

template <>
void WDTInterrupt::loop()
{
	WDTCR |= _BV(WDIE);
	_loop();
	WDTCR &= ~_BV(WDIE);
}

template <>
void WDTInterrupt::sleep()
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();

	if (WDTCR & _BV(WDIF)) {
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
