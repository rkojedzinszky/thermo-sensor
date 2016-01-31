#pragma once

#include <avr/eeprom.h>

uint8_t crc8_ccitt(const uint8_t* buf, uint8_t len);

