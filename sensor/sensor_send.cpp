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

	USICR = 0;
	uint16_t d;
	if (htu21d::read_temp(d)) {
		dp += SensorValue::HTU21DTemperature::encode(d, dp);
	}
	if (htu21d::read_hum(d)) {
		dp += SensorValue::HTU21DHumidty::encode(d, dp);
	}

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

