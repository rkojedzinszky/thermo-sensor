#pragma once

#include <stdint.h>

template <int N>
class LFSR {
	uint8_t reg_;
public:
	LFSR(uint8_t seed = 1) : reg_(seed) { }

	constexpr int bits() const {
		return N;
	}

	void set(uint8_t val) {
		reg_ = val & ((1 << N) - 1);
		if (reg_ == 0) {
			reg_ = 1;
		}
	}

	uint8_t get() {
		reg_ |= ((reg_ & 1) ^ ((reg_ & 2) >> 1)) << N;
		reg_ >>= 1;

		return reg_;
	}
};

