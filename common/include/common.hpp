#pragma once

#include <avr/eeprom.h>

uint8_t crc8_ccitt(const uint8_t* buf, uint8_t len);

struct Radiopacket {
	union {
		struct {
			unsigned short magic;
			unsigned short seq;
			unsigned char len;
			unsigned char id;
			unsigned char data[0];
		} __attribute__ ((__packed__));
		unsigned char raw[16];
	};
};

struct Radiopacket_status : public Radiopacket {
public:
	char rssi;
	unsigned char lqi;
};
