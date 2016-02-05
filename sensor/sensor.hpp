#pragma once

extern "C" {
#include <aes/aes.h>
};

#include <lfsr.hpp>
#include <port.hpp>
#include <usi.hpp>
#include <radio.hpp>
#include <am2302.hpp>

typedef AM2302<Pin<Port<B>, 4>> am2302;
typedef Radio<CC1101::CC1101<USI, Pin<Port<B>, 3>>> radio;

extern template class CC1101::CC1101<USI, Pin<Port<B>, 3>>;

class Main {
public:
	void init();

	void loop();

private:
	uint8_t random_wdt_delay() {
		uint8_t rnd = lfsr.get() & 7;

		if (rnd == 7) {
			rnd = _BV(WDIE) | _BV(WDP3); // 4 sec
		} else {
			rnd = _BV(WDIE) | (rnd + 1);
		}

		return rnd;
	}
	void send();

	uint16_t seq_ = 0;
	unsigned short magic_;
	unsigned char id_;
	aes128_ctx_t aes_ctx_;

	LFSR<7> lfsr;
};
