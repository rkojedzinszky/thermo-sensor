#pragma once

#include <avr/io.h>

typedef volatile uint8_t mmio8_type;

enum PortId {
	B
};

template <PortId P>
class Port {
public:
	static mmio8_type& port();
	static mmio8_type& pin();
	static mmio8_type& ddr();
};

template <>
inline mmio8_type& Port<B>::port()
{
	return PORTB;
}

template <>
inline mmio8_type& Port<B>::pin()
{
	return PINB;
}

template <>
inline mmio8_type& Port<B>::ddr()
{
	return DDRB;
}

enum PinMode {
	INPUT,
	OUTPUT
};

template <class Port, int pin>
class Pin {
public:
	// reads PIN
	static char value() {
		return Port::pin() & _BV(pin);
	}

	// set high
	static void set() {
		Port::port() |= _BV(pin);
	}

	// set low
	static void clear() {
		Port::port() &= ~_BV(pin);
	}

	// write PORT
	static void output(char v) {
		if (v) {
			set();
		} else {
			clear();
		}
	}

	// write DDR
	static void mode(PinMode mode) {
		if (mode == OUTPUT) {
			Port::ddr() |= _BV(pin);
		} else {
			Port::ddr() &= ~_BV(pin);
		}
	}

	// from sfr_defs
	static bool is_set() {
		return bit_is_set(Port::pin(), pin);
	}
	static bool is_clear() {
		return bit_is_clear(Port::pin(), pin);
	}
	static void loop_until_set() {
		loop_until_bit_is_set(Port::pin(), pin);
	}
	static void loop_until_clear() {
		loop_until_bit_is_clear(Port::pin(), pin);
	}
	static bool loop_until_set(unsigned char timeout_us) {
		asm volatile(
				"1: sbic %2, %3\n"
				"rjmp 2f\n"
				"dec %0\n"
				"brne 1b\n"
				"2:\n"
				: "=r" (timeout_us)
				: "0" (timeout_us / 5), "i" (_SFR_IO_ADDR(Port::pin())), "i" (pin)
			    );
		return timeout_us != 0;
	}
	static bool loop_until_clear(unsigned char timeout_us) {
		asm volatile(
				"1: sbis %2, %3\n"
				"rjmp 2f\n"
				"dec %0\n"
				"brne 1b\n"
				"2:\n"
				: "=r" (timeout_us)
				: "0" (timeout_us / 5), "i" (_SFR_IO_ADDR(Port::pin())), "i" (pin)
			    );
		return timeout_us != 0;
	}
};

