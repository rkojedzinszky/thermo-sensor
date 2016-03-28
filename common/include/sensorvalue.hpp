#pragma once

#include <stdint.h>

// Sensor types
// first byte
// M......L
// 0Txxxxxx - T type,  2 types, 14 bit raw data - T0
// 10TTxxxx - T type,  4 types, 12 bit raw data - T1
// 110TTTxx - T type,  8 types, 10 bit raw data - T2
// 1110TTTT - T type, 16 types,  8 bit raw data - T3

namespace SensorValue {

template <int T, int t>
class Value {
public:
	static constexpr uint8_t encoded_length() {
		return 2;
	}

	static constexpr uint8_t header() {
		return static_cast<uint8_t>((0xff << (8 - T)) | ((t & (0xff >> (7 - T))) << (6 - (2*T))));
	}

	static constexpr uint8_t d0mask() {
		return 0xff >> (2*(T+1));
	}

	static uint8_t encode(short value, uint8_t* p) {
		p[0] = header() | ((value >> 8) & d0mask());
		p[1] = value & 0xff;

		return encoded_length();
	}
};

// T0
typedef Value<0, 0> HTU21DTemperature;
// T1
typedef Value<1, 0> HTU21DHumidty;
typedef Value<1, 1> AM2302Temperature;
// T2
typedef Value<2, 0> VCC;
typedef Value<2, 1> AM2302Humidity;
// T3
typedef Value<3, 0> RSSI;
typedef Value<3, 1> LQI;

};
