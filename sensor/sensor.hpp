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
extern template class AM2302<Pin<Port<B>, 4>>;

class Sensor {
public:
	void init();

	void loop();

private:
	void send();

	uint16_t seq_ = 0;
	unsigned short magic_;
	unsigned char id_;
	aes128_ctx_t aes_ctx_;
};

static inline void _thermo_on(bool on)
{
	radio::set(CC1101::IOCFG2, on ? 0x6f : 0x2e);
}

static inline void thermo_on(bool on)
{
	radio::select();
	_thermo_on(on);
	radio::wcmd(CC1101::SPWD);
	radio::release();
}
