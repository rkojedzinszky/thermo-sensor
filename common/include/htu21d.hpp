#pragma once

#include <stdint.h>
#include <avr/cpufunc.h>
#include <avr/sleep.h>
#include <i2c.hpp>
#include <common.hpp>

class HTU21D {
public:
	static bool reset();
	static bool read_temp(uint16_t& temp);
	static bool read_hum(uint16_t& hum);
	static void seti2c(I2CI* i2c);

private:
	static I2CI* i2c;
	static bool _reset();
	static bool _read(uint8_t cmd, uint16_t& m);
};
