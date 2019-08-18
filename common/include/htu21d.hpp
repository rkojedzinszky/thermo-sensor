#pragma once

#include <stdint.h>
#include <avr/cpufunc.h>
#include <avr/sleep.h>
#include <i2c.hpp>
#include <common.hpp>

// port state terminology:
// low - output/low
// high - output/high
// input - input will pull-up

class IHTU21D {
public:
	virtual bool reset() = 0;
	virtual bool read_temp(uint16_t& temp) = 0;
	virtual bool read_hum(uint16_t& hum) = 0;
};

template <typename CL_t, typename DA_t>
class HTU21D : public IHTU21D {
public:
	typedef CL_t CL;
	typedef DA_t DA;
	typedef I2C<CL_t, DA_t> i2c;

	virtual bool reset();
	virtual bool read_temp(uint16_t& temp);
	virtual bool read_hum(uint16_t& hum);

private:
	static bool _reset();
	static bool _read(uint8_t cmd, uint16_t& m);
};

template <typename CL_t, typename DA_t>
bool HTU21D<CL_t, DA_t>::reset()
{
	i2c::start();
	i2c::stop();
	i2c::start();
	bool ret = _reset();
	i2c::stop();

	return ret;
}

template <typename CL_t, typename DA_t>
bool HTU21D<CL_t, DA_t>::read_temp(uint16_t& temp)
{
	bool ret;

	i2c::start();
	ret = _read(0xe3, temp);
	i2c::stop();

	if (ret == false)
		return false;

	temp >>= 2;

	return true;
}

template <typename CL_t, typename DA_t>
bool HTU21D<CL_t, DA_t>::read_hum(uint16_t& hum)
{
	bool ret;

	i2c::start();
	ret = _read(0xe5, hum);
	i2c::stop();

	if (ret == false)
		return false;

	hum >>= 4;

	return true;
}

template <typename CL_t, typename DA_t>
inline bool HTU21D<CL_t, DA_t>::_reset()
{
	if (!i2c::send_byte(0x80))
		return false;

	if (!i2c::send_byte(0xe6))
		return false;

	if (!i2c::send_byte(0x02))
		return false;

	i2c::stop();
	_NOP();
	i2c::start();

	if (!i2c::send_byte(0x80))
		return false;

	if (!i2c::send_byte(0xfe))
		return false;

	return true;
}

template <typename CL_t, typename DA_t>
bool HTU21D<CL_t, DA_t>::_read(uint8_t cmd, uint16_t& m)
{
	if (!i2c::send_byte(0x80))
		return false;

	if (!i2c::send_byte(cmd))
		return false;

	i2c::stop();
	_NOP();
	i2c::start();

	if (!i2c::send_byte(0x81))
		return false;

	i2c::CL::set();

	PCMSK = _BV(CL::pin);
	GIMSK |= _BV(PCIE);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	GIMSK &= ~_BV(PCIE);

	uint8_t buf[3];

	buf[0] = i2c::recv_byte();
	buf[1] = i2c::recv_byte();
	buf[2] = i2c::recv_byte(true);

	if (crc8_dallas(0, buf, 2) != buf[2])
		return false;

	m = buf[0] << 8 | buf[1];

	return true;
}
