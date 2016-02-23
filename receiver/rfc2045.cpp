#include "rfc2045.h"

uint8_t RFC2045::encode(const uint8_t* src, uint8_t slen, uint8_t* dst)
{
	uint8_t* d = dst;
	uint8_t i = 0;
	uint8_t t;

	while (slen-- > 0) {
		switch(i) {
		case 0:
			*dst++ = map(*src / 4);
			t = (*src * 16) & 0x30;
			i = 1;
			break;
		case 1:
			*dst++ = map(t + *src / 16);
			t = (*src * 4) & 0x3c;
			i = 2;
			break;
		case 2:
			*dst++ = map(t + *src / 64);
			*dst++ = map(*src & 0x3f);
			i = 0;
			break;
		}

		src++;
	}

	if (i > 0) {
		*dst++ = map(t);
		*dst++ = '=';
		if (i == 1)
			*dst++ = '=';
	}

	return dst - d;
}

uint8_t RFC2045::map(const uint8_t c)
{
	if (c < 26)
		return 'A' + c;

	if (c < 52)
		return 'a' - 26 + c;

	if (c < 62)
		return '0' - 52 + c;

	if (c == 62)
		return '+';

	if (c == 63)
		return '/';

	return '?';
}
