#pragma once

#include <avr/interrupt.h>

struct Callback {
	void (*func)() = nullptr;

	operator bool() const {
		return func != nullptr;
	}

	void operator()() {
		(*func)();
		func = nullptr;
	}
};

template <int Interrupt_num>
class Interrupt {
public:
	static void enqueue(void (*f)());
	static void loop();
	static void sleep();
	static void tick();

private:
	static Callback queued_;
	static Callback pending_;

	static void _loop();
};

typedef class Interrupt<WDT_vect_num> WDTInterrupt;
typedef class Interrupt<TIM0_OVF_vect_num> TIM0Interrupt;
