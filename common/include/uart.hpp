#pragma once

#include <fifo.hpp>

/*
 * \class Uart
 *
 * \brief SW implemented UART @ 9600 baud
 * with 8MHz system clock
 *
 * constants are hard-coded
 * You will call either rx_int or tx_int from an interrupt handler, but not both
 * This will determine the uart's direction
 */

template <class Pin_t, int Fifo_E = 3>
class Uart {
public:
	/// [rt]x_int is expected to be called at 4x baud rate

	/// These are for RX direction
	bool rx(unsigned char& byte);
	void rx_int();

	/// These are for TX direction
	bool tx(unsigned char byte);
	void tx_int();

	typedef Pin_t Pin;

private:
	static constexpr int bittimer = 4;
	unsigned char timer;
	unsigned char bit = 0;

	typedef Fifo<unsigned char, Fifo_E> Fifo_t;
	Fifo_t fifo;
};

template <class Pin_t, int Fifo_E>
inline bool Uart<Pin_t, Fifo_E>::rx(unsigned char& byte)
{
	if (fifo.empty())
		return false;

	byte = fifo.head();
	fifo.pop();
	return true;
}

template <class Pin_t, int Fifo_E>
inline void Uart<Pin_t, Fifo_E>::rx_int()
{
	if (bit != 0) {
		if (--timer > 0)
			return;

		--bit;
	}

	if (bit == 0) { // waiting for start bit
		if (Pin::is_clear()) { // stop detected
			bit = 10;
			timer = bittimer + 1;
		}
		return;
	}

	if (bit == 1) { // stop bit
		if (Pin::is_set()) {
			fifo.push();
		}
		timer = 1;
	} else { // data bit
		fifo.tail() >>= 1;
		if (Pin::is_set()) {
			fifo.tail() |= 0x80;
		}
		timer = bittimer;
	}
}

template <class Pin_t, int Fifo_E>
inline bool Uart<Pin_t, Fifo_E>::tx(unsigned char byte)
{
	if (fifo.full())
		return false;

	fifo.tail() = byte;
	fifo.push();
	return true;
}

template <class Pin_t, int Fifo_E>
inline void Uart<Pin_t, Fifo_E>::tx_int()
{
	if (bit != 0) {
		if (--timer > 0)
			return;

		if (--bit == 0)
			fifo.pop();
	}

	if (bit == 0) { // check for new tx byte
		if (!fifo.empty()) {
			Pin::clear();

			bit = 10;
			timer = bittimer;
		}
		return;
	}

	if (bit == 1) { // stop bit
		Pin::set();
	} else { // data bit
		if (fifo.head() & 1) {
			Pin::set();
		} else {
			Pin::clear();
		}
		fifo.head() >>= 1;
	}
	timer = bittimer;
}
