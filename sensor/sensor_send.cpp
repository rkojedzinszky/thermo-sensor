#include <sensorvalue.hpp>
#include <vcc.hpp>
#include <packet.hpp>
#include "sensor.hpp"

void Sensor::send()
{
	Radiopacket packet;

	unsigned char* dp = packet.data;

	unsigned vl = VCC::read_voltage();
	dp += SensorValue::VCC::encode(vl, dp);

	short hum, temp;
	if (am2302::read(hum, temp)) {
		dp += SensorValue::AM2302Humidity::encode(hum, dp);
		dp += SensorValue::AM2302Temperature::encode(temp, dp);
	}

	packet.magic = magic_;
	packet.len = dp - packet.raw;
	packet.id = id_;
	packet.seq = seq_++;

	::aes128_enc(packet.raw, &aes_ctx_);

	radio::select();
	_thermo_on(false);
	radio::set(CC1101::MCSM0, 0x30);
	radio::set(CC1101::IOCFG1, 0x06);
	radio::wcmd(CC1101::STX);
	radio::write_txfifo(packet.raw, sizeof(packet.raw));
}

