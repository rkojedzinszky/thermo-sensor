#pragma once

#if defined(__AVR_ATtiny85__)
#include <vcc_tnx5.hpp>
#define VCC_impl VCC_tnx5
#endif

class VCC {
	VCC_impl impl_;
public:
	int read_voltage() {
		return impl_.read_voltage();
	}
};
