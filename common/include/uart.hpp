#pragma once

#include <fifo.hpp>

/*
 * \class Uart
 *
 * \brief SW implemented Full duplex UART @ 9600 baud
 * with 8MHz system clock
 *
 * Many constants are hard-coded
 */

template <class RX_t, class TX_t, int E_rx = 3, int E_tx = 3>
class Uart {
public:
	/// This is expected to be called at 4x baud rate
	static void timertick();
	static bool rx(unsigned char& byte);
	static bool tx(unsigned char byte);

	typedef RX_t RX;
	typedef TX_t TX;

	typedef Fifo<unsigned char, E_rx> RXFifo_t;
	typedef Fifo<unsigned char, E_tx> TXFifo_t;
private:
	static constexpr int bittimer = 4;

	static void handle_rx();
	static void handle_tx();

	static unsigned char rxtimer;
	static unsigned char rxbit;
	static RXFifo_t rxfifo;

	static unsigned char txtimer;
	static unsigned char txbit;
	static TXFifo_t txfifo;
};

template <class RX, class TX, int E_rx, int E_tx>
inline void Uart<RX, TX, E_rx, E_tx>::timertick()
{
	handle_rx();
	handle_tx();
}

template <class RX, class TX, int E_rx, int E_tx>
inline bool Uart<RX, TX, E_rx, E_tx>::rx(unsigned char& byte)
{
	if (!rxfifo.empty()) {
		byte = rxfifo.head();
		rxfifo.pop();
		return true;
	}

	return false;
}

template <class RX, class TX, int E_rx, int E_tx>
inline bool Uart<RX, TX, E_rx, E_tx>::tx(unsigned char byte)
{
	if (!txfifo.full()) {
		txfifo.tail() = byte;
		txfifo.push();
		return true;
	}

	return false;
}

template <class RX, class TX, int E_rx, int E_tx>
inline void Uart<RX, TX, E_rx, E_tx>::handle_rx()
{
	if (rxbit != 0) {
		if (--rxtimer > 0)
			return;

		--rxbit;
	}

	if (rxbit == 0) { // waiting for start bit
		if (RX::is_clear()) { // stop detected
			rxbit = 10;
			rxtimer = bittimer;
		}
		return;
	}

	if (rxbit == 1) { // stop bit
		if (RX::is_set()) {
			rxfifo.push();
		}
		rxtimer = 1;
	} else { // data bit
		rxfifo.tail() >>= 1;
		if (RX::is_set()) {
			rxfifo.tail() |= 0x80;
		}
		rxtimer = bittimer;
	}
}

template <class RX, class TX, int E_rx, int E_tx>
inline void Uart<RX, TX, E_rx, E_tx>::handle_tx()
{
	if (txbit != 0) {
		if (--txtimer > 0)
			return;

		if (--txbit == 0)
			txfifo.pop();
	}

	if (txbit == 0) { // check for new tx byte
		if (!txfifo.empty()) {
			TX::clear();

			txbit = 10;
			txtimer = bittimer;
		}
		return;
	}

	if (txbit == 1) { // stop bit
		TX::set();
	} else { // data bit
		if (txfifo.head() & 1) {
			TX::set();
		} else {
			TX::clear();
		}
		txfifo.head() >>= 1;
	}
	txtimer = bittimer;
}

#define Uart_declare_members(uart) \
	template <> unsigned char uart::rxtimer = 0; \
	template <> unsigned char uart::rxbit = 0; \
	template <> uart::RXFifo_t uart::rxfifo = uart::RXFifo_t(); \
	template <> unsigned char uart::txtimer = 0; \
	template <> unsigned char uart::txbit = 0; \
	template <> uart::TXFifo_t uart::txfifo = uart::TXFifo_t();

