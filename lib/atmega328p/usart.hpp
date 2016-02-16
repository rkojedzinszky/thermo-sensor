#pragma once

#include <avr/io.h>
#include <fifo.hpp>

template <int rxfifo_E, int txfifo_E>
class Usart {
public:
	/// Initialization
	static void init();

	/// These are for RX direction
	static bool rx(unsigned char& byte);
	static void rx_ready();

	/// These are for TX direction
	static bool tx(unsigned char byte);
	static void tx_ready();

private:
	typedef Fifo<unsigned char, rxfifo_E> rxfifo_t;
	typedef Fifo<unsigned char, txfifo_E> txfifo_t;
	static rxfifo_t rxfifo;
	static txfifo_t txfifo;

};

template <int rxfifo_E, int txfifo_E>
inline void Usart<rxfifo_E, txfifo_E>::init()
{
	UBRR0H = 0;
	UBRR0L = 25;

	UCSR0A = _BV(U2X0);
	UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

template <int rxfifo_E, int txfifo_E>
bool Usart<rxfifo_E, txfifo_E>::rx(unsigned char& byte)
{
	if (rxfifo.empty())
		return false;

	byte = rxfifo.head();
	rxfifo.pop();
	return true;
}

template <int rxfifo_E, int txfifo_E>
bool Usart<rxfifo_E, txfifo_E>::tx(unsigned char byte)
{
	if (txfifo.full())
		return false;

	if (txfifo.empty() && (UCSR0A & _BV(UDRE0))) {
		UDR0 = byte;
	} else {
		txfifo.tail() = byte;
		txfifo.push();

		UCSR0B |= _BV(UDRIE0);
	}

	return true;
}

// call it from USART_RX_vect
template <int rxfifo_E, int txfifo_E>
inline void Usart<rxfifo_E, txfifo_E>::rx_ready()
{
	if (!rxfifo.full()) {
		rxfifo.tail() = UDR0;
		rxfifo.push();
	}
}

// call it from USART_UDRE_vect
template <int rxfifo_E, int txfifo_E>
inline void Usart<rxfifo_E, txfifo_E>::tx_ready()
{
	if (!txfifo.empty()) {
		UDR0 = txfifo.head();
		txfifo.pop();
	} else {
		UCSR0B &= ~_BV(UDRE0);
	}
}

