#include "spi_tx5.hpp"

unsigned char SPI_tx5::xmit(unsigned char data)
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
