#pragma once

#include <stdint.h>

template <int n>
inline uint8_t asr(uint8_t reg)
{
	asm volatile(
			"asr %1\n"
			: "+r" (reg)
		    );
	return asr<n-1>(reg);
}

template <>
inline uint8_t asr<0>(uint8_t reg)
{
	return reg;
}
