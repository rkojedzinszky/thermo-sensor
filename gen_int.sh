#!/bin/sh

set -e

src="$1"
test -f "$src"
test -f ../../../common/include/interrupt/base

gen_int()
{
	local name="$1"
	cat > $int.hpp <<EOF
#pragma once

#include <avr/io.h>
#include <interrupt/base>

typedef Interrupt<${int}_vect_num> ${int}Interrupt;
EOF

	cat > $int.cpp <<EOF
#include <avr/interrupt.h>
#include <interrupt/${int}.hpp>

ISR(${int}_vect, ISR_NAKED)
{
	asm(
		"push r24\\n"
		"ldi r24, 1\\n"
		"sts %0, r24\\n"
		"pop r24\\n"
		"reti\\n"
		:
		: "m" (${int}Interrupt::fire_)
	);
}

template <>
volatile bool ${int}Interrupt::fire_ = false;

template <>
callback_t ${int}Interrupt::callback_ = nullptr;
EOF
}

rm -f *.cpp *.hpp

cat > CMakeLists.txt <<EOF
# generated as: $0 $@
add_library("\${MCU}_interrupt" STATIC
EOF

for int in $(sed -n -r -e '/_vect_num/{s/^.*define[[:space:]]+//; s/_vect_num.*//; p}' "$src"); do
	gen_int $int
	echo " $int.cpp" >> CMakeLists.txt
done

echo ")" >> CMakeLists.txt
