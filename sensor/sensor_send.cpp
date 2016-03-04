#include <sensorvalue.hpp>
#include <vcc.hpp>
#include <packet.hpp>
#include "sensor.hpp"

void Sensor::send()
{
	Radiopacket packet;

	VCC vccreader;
	unsigned char* dp = packet.data;

	short hum, temp;
	if (am2302::read(hum, temp)) {
		dp += SensorValue<Humidity>::encode(hum, dp);
		dp += SensorValue<Temperature>::encode(temp, dp);
	}

	unsigned vl = vccreader.read_voltage();
	dp += SensorValue<Power>::encode(vl, dp);

	packet.magic = magic_;
	packet.len = dp - packet.raw;
	packet.id = id_;
	packet.seq = seq_++;

	::aes128_enc(packet.raw, &aes_ctx_);

	radio::select();
	radio::set(CC1101::IOCFG1, 0x86);
	radio::wcmd(CC1101::STX);
	radio::write_txfifo(packet.raw, sizeof(packet.raw));
}

