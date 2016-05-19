#pragma once

#include <avr/eeprom.h>
#include <config.hpp>

struct Radiopacket {
	union {
		struct {
			uint16_t magic;
			int32_t seq; // signed, but no negative values are allowed
			uint8_t len;
			uint8_t id;
			uint8_t data[0];
		} __attribute__ ((__packed__));
		uint8_t raw[16];
	};
};

struct Radiopacket_status : public Radiopacket {
public:
	char rssi;
	unsigned char lqi;
};

struct Packet {
	uint8_t len_ = 1;
	uint8_t dst_ = 0;

	Packet() { }
	Packet(uint8_t dst) : dst_(dst) { }
	Packet(uint8_t dst, uint8_t len) : len_(1 + len), dst_(dst) { }
} __attribute__ ((__packed__));

struct ConfigRequestPacket : public Packet {
	uint8_t src_;

	ConfigRequestPacket() { }
	ConfigRequestPacket(uint8_t src) : Packet(0, 1), src_(src) { }
} __attribute__ ((__packed__));

struct ConfigResponsePacket : public Packet {
	Config config_;

	ConfigResponsePacket() { }
	ConfigResponsePacket(uint8_t dst, const Config& config) : Packet(dst, sizeof(Config)), config_(config) { }
} __attribute__ ((__packed__));
