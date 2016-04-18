#pragma once

#include <port.hpp>

template <typename P_t>
class I2CW {
public:
	typedef P_t P;

	// input/pullup
	static void set();
	// output/low
	static void clear();

	static bool is_set();
	static bool is_clear();
};

template <typename P_t>
inline void I2CW<P_t>::set()
{
	P::set();
	P::mode(INPUT);
}

template <typename P_t>
inline void I2CW<P_t>::clear()
{
	P::clear();
	P::mode(OUTPUT);
}

template <typename P_t>
inline bool I2CW<P_t>::is_set()
{
	return P::is_set();
}

template <typename P_t>
inline bool I2CW<P_t>::is_clear()
{
	return P::is_clear();
}

template <typename CL_t, typename DA_t>
class I2C {
public:
	typedef I2CW<CL_t> CL;
	typedef I2CW<DA_t> DA;

	static void start();
	static void stop();
	static void send_bit(bool bit);
	static bool recv_bit();
	static bool send_byte(uint8_t data);
	static uint8_t recv_byte(bool nack = false);
};

template <typename CL_t, typename DA_t>
inline void I2C<CL_t, DA_t>::start()
{
	DA::clear();
	CL::clear();
}

template <typename CL_t, typename DA_t>
inline void I2C<CL_t, DA_t>::stop()
{
	DA::clear();
	CL::set();
	DA::set();
}

template <typename CL_t, typename DA_t>
inline void I2C<CL_t, DA_t>::send_bit(bool bit)
{
	if (bit) {
		DA::set();
	} else {
		DA::clear();
	}

	CL::set();
	while (CL::is_clear());
	CL::clear();
}

template <typename CL_t, typename DA_t>
inline bool I2C<CL_t, DA_t>::recv_bit()
{
	CL::set();
	while (CL::is_clear());
	bool bit = DA::is_set();
	CL::clear();

	return bit;
}

template <typename CL_t, typename DA_t>
bool I2C<CL_t, DA_t>::send_byte(uint8_t data)
{
	for (uint8_t i = 0; i < 8; ++i) {
		send_bit(data & 0x80);

		data += data;
	}

	DA::set();

	return recv_bit() == false;
}

template <typename CL_t, typename DA_t>
uint8_t I2C<CL_t, DA_t>::recv_byte(bool nack)
{
	uint8_t data;

	DA::set();

	for (uint8_t i = 0; i < 8; ++i) {
		data += data;

		if (recv_bit())
			data |= 1;
	}

	send_bit(nack);

	return data;
}
