#pragma once

#include <stdint.h>
#include <bitwise.hpp>

template <int N>
class LFSR {
	uint8_t reg_;
public:
	LFSR(uint8_t seed = 1) : reg_(seed) { }

	static constexpr int bits() {
		return N;
	}

	void set(uint8_t val) {
		reg_ = val & ((1 << N) - 1);
		if (reg_ == 0) {
			reg_ = 1;
		}
	}

	uint8_t get() {
		asm volatile(
				"ldi r24, %1\n"
				"sbrc %0, 0\n"
				"eor %0, r24\n"
				"sbrc %0, 1\n"
				"eor %0, r24\n"
				"asr %0\n"
				: "+r" (reg_)
				: "i" (1 << N)
				: "r24"
			    );

		return reg_;
	}
};

