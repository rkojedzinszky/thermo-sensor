#pragma once

#include <avr/eeprom.h>

uint8_t crc8_dallas(uint8_t __crc, const uint8_t* __buf, uint8_t __len);
