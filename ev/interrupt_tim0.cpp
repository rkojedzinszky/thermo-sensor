#include <avr/sleep.h>
#include "interrupt_impl.hpp"

template <>
void TIM0Interrupt::enqueue(void (*f)())
{
	_enqueue(f);
	TIMSK |= _BV(TOIE0);
}

template <>
void TIM0Interrupt::loop()
{
	_loop();
	TIMSK &= ~_BV(TOIE0);
}

template <>
void TIM0Interrupt::sleep()
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();

	if (bit_is_set(TIFR, TOV0)) {
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
