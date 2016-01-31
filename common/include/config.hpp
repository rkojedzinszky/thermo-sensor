#pragma once

#include <avr/eeprom.h>
#include <cc1101.hpp>
#include <common.hpp>

class Config {
public:
	static constexpr int MaxRadioConfigEntries = 16;

	struct RadioConfig {
		enum CC1101::ConfReg reg;
		uint8_t value;
	} __attribute__ ((__packed__));

	uint8_t& id() {
		return data_.id_;
	}
	uint16_t& magic() {
		return data_.magic_;
	}
	uint8_t* key() {
		return data_.key_;
	}
	RadioConfig* radioconfig() {
		return data_.radioconfig_;
	}

	void read() {
		eeprom_read_block(this, &config_, sizeof(*this));
	}
	void write() {
		crc8 = calculate_crc();
		eeprom_write_block(this, &config_, sizeof(*this));
	}
	bool valid() const {
		return crc8 == calculate_crc();
	}

private:
	uint8_t calculate_crc() const {
		return crc8_ccitt(data_.raw_, sizeof(data_));
	}

	uint8_t crc8;
	union {
		struct {
			uint8_t id_;
			uint16_t magic_;
			uint8_t key_[16];
			RadioConfig radioconfig_[MaxRadioConfigEntries];
		};
		uint8_t raw_[1];
	} data_;

	static Config config_;
} __attribute__ ((__packed__));
