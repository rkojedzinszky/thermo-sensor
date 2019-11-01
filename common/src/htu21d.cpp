#include <htu21d.hpp>

I2CI* HTU21D::i2c = 0;

void HTU21D::seti2c(I2CI* i2c_)
{
	i2c = i2c_;

	if (i2c == 0)
		return;

	i2c->start();
	i2c->stop();
}

bool HTU21D::reset()
{
	if (i2c == 0)
		return false;

	i2c->start();
	bool ret = _reset();
	i2c->stop();

	return ret;
}

bool HTU21D::read_temp(uint16_t& temp)
{
	if (i2c == 0)
		return false;

	bool ret;

	i2c->start();
	ret = _read(0xe3, temp);
	i2c->stop();

	if (ret == false)
		return false;

	temp >>= 2;

	return true;
}

bool HTU21D::read_hum(uint16_t& hum)
{
	if (i2c == 0)
		return false;

	bool ret;

	i2c->start();
	ret = _read(0xe5, hum);
	i2c->stop();

	if (ret == false)
		return false;

	hum >>= 4;

	return true;
}

bool HTU21D::_reset()
{
	static uint8_t wruserreg[] = {0x80, 0xe6, 0x02};

	if (!i2c->send_bytes(wruserreg, 3))
		return false;

	i2c->stop();
	_NOP();
	i2c->start();

	static uint8_t softreset[] = {0x80, 0xfe};
	if (!i2c->send_bytes(softreset, 2))
		return false;

	return true;
}

bool HTU21D::_read(uint8_t cmd, uint16_t& m)
{
	uint8_t readcmd[2] = {0x80, cmd};

	if (!i2c->send_bytes(readcmd, 2))
		return false;

	i2c->stop();
	_NOP();
	i2c->start();

	readcmd[0] = 0x81;
	if (!i2c->send_bytes(readcmd, 1))
		return false;

	i2c->CLset();

	PCMSK = _BV(i2c->CLpin());
	GIMSK |= _BV(PCIE);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	GIMSK &= ~_BV(PCIE);
	PCMSK = 0;

	uint8_t buf[3];

	i2c->recv_bytes(buf, 3);

	if (crc8_dallas(0, buf, 2) != buf[2])
		return false;

	m = buf[0] << 8 | buf[1];

	return true;
}
