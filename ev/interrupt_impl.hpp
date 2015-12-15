#pragma once

#include "interrupt.hpp"

template <int Interrupt_num>
inline void Interrupt<Interrupt_num>::_enqueue(void (*f)())
{
	queued_.func = f;
}

template <int Interrupt_num>
inline void Interrupt<Interrupt_num>::_loop()
{
	while (queued_) {
		sleep();
		if (pending_) {
			pending_();
		}
	}
}

template <int Interrupt_num>
inline void Interrupt<Interrupt_num>::tick()
{
	pending_.func = queued_.func;
	queued_.func = nullptr;
}
