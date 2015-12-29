#include "spi_t84a.hpp"

unsigned char SPI_t84a::xmit(unsigned char data)
{
	unsigned char ch = _BV(USIWM0) | _BV(USITC);
	unsigned char cl = ch | _BV(USICLK);

	USIDR = data;
	USICR = ch;
	USICR = cl;
	USICR = ch;
	USICR = cl;
	USICR = ch;
	USICR = cl;
	USICR = ch;
	USICR = cl;
	USICR = ch;
	USICR = cl;
	USICR = ch;
	USICR = cl;
	USICR = ch;
	USICR = cl;
	USICR = ch;
	USICR = cl;
	return USIDR;
}
