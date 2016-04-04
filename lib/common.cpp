#include <stdint.h>
#include <util/crc16.h>
#include <common.hpp>

uint8_t crc8_dallas(uint8_t __crc, const uint8_t* __buf, uint8_t __len)
{
	uint8_t __i, __pattern;
	uint8_t __data;

	__asm__ __volatile__ (
		"    tst    %4" "\n\t"
		"    breq   3f" "\n\t"
		"    ldi    %2, 0x31" "\n\t"
		"    movw   r30, %5" "\n\t"
		"0:  ld     %3, Z+" "\n\t"
		"    eor    %0, %3" "\n\t"
		"    ldi    %1, 8" "\n\t"
		"1:  lsl    %0" "\n\t"
		"    brcc   2f" "\n\t"
		"    eor    %0, %2" "\n\t"
		"2:  dec    %1" "\n\t"
		"    brne   1b" "\n\t"
		"    dec    %4" "\n\t"
		"    brne   0b" "\n\t"
		"3:           " "\n\t"
		: "+r" (__crc), "=r" (__i), "=r" (__pattern), "=r" (__data), "+r" (__len)
		: "r" (__buf)
		: "r30", "r31" );

	return __crc;
}
