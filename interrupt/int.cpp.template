#include <avr/interrupt.h>
#include <interrupt/@NAME@.hpp>

ISR(@NAME@_vect)
{
	@NAME@Interrupt::tick();
}

template <>
volatile bool @NAME@Interrupt::fire_ = false;

template <>
callback_t @NAME@Interrupt::callback_ = nullptr;
