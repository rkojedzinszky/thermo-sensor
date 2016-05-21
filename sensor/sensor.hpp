#pragma once

#include <avr/wdt.h>

extern "C" {
#include <aes/aes.h>
};

#include <lfsr.hpp>
#include <port.hpp>
#include <usi.hpp>
#include <radio.hpp>
#include <htu21d.hpp>

typedef HTU21D<Pin<Port<B>, 4>, Pin<Port<B>, 1>> htu21d;
typedef Radio<CC1101::CC1101<USI, Pin<Port<B>, 3>>> radio;

extern template class CC1101::CC1101<USI, Pin<Port<B>, 3>>;

class Sensor {
public:
	void init();

	void loop();

private:
	void send();

	int32_t seq_;
	unsigned short magic_;
	unsigned char id_;
	aes128_ctx_t aes_ctx_;
	LFSR lfsr;
};

static constexpr int delay_multiplier = 1000 / 29;
