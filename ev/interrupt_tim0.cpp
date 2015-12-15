#include <avr/sleep.h>
#include "interrupt_impl.hpp"

template <>
void TIM0Interrupt::loop()
{
	TIMSK |= _BV(TOIE0);
	_loop();
	TIMSK &= ~_BV(TOIE0);
}

template <>
void TIM0Interrupt::sleep()
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();

	if (TIFR & _BV(TOV0)) {
		tick();
		TIFR &= ~_BV(TOV0);
	}
}

template class Interrupt<TIM0_OVF_vect_num>;

ISR(TIM0_OVF_vect)
{
	TIM0Interrupt::tick();
}

template<>
Callback TIM0Interrupt::queued_ = Callback();

template<>
Callback TIM0Interrupt::pending_ = Callback();
