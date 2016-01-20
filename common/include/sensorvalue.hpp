#pragma once

// Sensor types represented in 3 bits
enum Sensor_Basic {
	Power = 0,		// mV
	Humidity,		// tenth relative percent
	Temperature,		// tenth Celsius
	RSSI,			// sensor RSSI
	LQI,			// sensor LQI
	Extended = 0x7
};

template <Sensor_Basic T>
class SensorValue {
public:
	static int encoded_length() {
		return 2;
	}

	static int encode(int value, unsigned char* p) {
		p[0] = (T << 5) | ((value >> 8) & 0x1f);
		p[1] = value & 0xff;

		return encoded_length();
	}
};
