#pragma once

#include <stdint.h>

class LFSR {
	uint16_t reg_ = 1;
	uint16_t xor_ = 1;
public:
	void set(uint16_t reg) {
		if (reg != 0) {
			reg_ = xor_ = reg;
		}
	}

	uint16_t get() {
		asm volatile(
				"ldi r23, 1\n"
				"clr __tmp_reg__\n"
				"sbrc %A0, 5\n"
				"eor __tmp_reg__, r23\n"
				"sbrc %A0, 3\n"
				"eor __tmp_reg__, r23\n"
				"sbrc %A0, 2\n"
				"eor __tmp_reg__, r23\n"
				"sbrc %A0, 0\n"
				"eor __tmp_reg__, r23\n"
				"lsr __tmp_reg__\n"
				"ror %B0\n"
				"ror %A0\n"
				: "+r" (reg_)
				:
				: "r23"
			    );

		return reg_ ^ xor_;
	}
};

