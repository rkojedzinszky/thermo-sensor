#pragma once

#include <avr/eeprom.h>

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

class Deviceconfig {
public:
	unsigned short magic;
	unsigned char key[16];
	unsigned char id;

	void read() {
		eeprom_read_block(this, &config, sizeof(*this));
	}
private:
	static Deviceconfig config;
} __attribute__ ((__packed__));
