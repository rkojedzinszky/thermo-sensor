#include "spi.hpp"

unsigned char SPI::xmit(unsigned char data)
{
	SPDR = data;
	loop_until_bit_is_set(SPSR, SPIF);
	return SPDR;
}
