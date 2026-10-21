#include "haptic.h"

void Haptic::begin_packet() {
	UDP.beginPacket(Address, Port);
	UDP.write(0);
	UDP.write(0);
	UDP.write(0);
}

template <typename Packet>
void Haptic::send(PacketType type, Packet packet) {

	size_t size = sizeof(packet);
	uint8_t data[size];

	memcpy(&data, &packet, size);

	begin_packet();

	UDP.write(UINT8(type));
	UDP.write(data, size);

	UDP.endPacket();
}

void Haptic::send_ping() {

	begin_packet();

	UDP.write(UINT8(PacketType::Ping));

	UDP.endPacket();
}

void Haptic::send_discovery() {

	DiscoveryPacket packet;
	WiFi.macAddress(packet.mac);

	Haptic::send(
		PacketType::Discovery,
		packet
	);
}

void Haptic::setup() {

	UDP.begin(Port);

	if (!hasPin) return;

	pinMode(MOTOR_PIN, OUTPUT);

#ifdef ESP8266
	analogWriteFreq(2000);
	analogWriteRange(255);
#else
	analogWriteFrequency(MOTOR_PIN, 2000);
	analogWriteResolution(MOTOR_PIN, 8);
#endif

}

void Haptic::update() {

	if (hasPin) {

		if (LastDutyTime + MOTOR_TIMEOUT < millis()) {

			if (Testing) {
				Logger.info("Testing Done!");
				Testing = false;
			}

			CurrentDuty = DUTY_ZERO;
		}

		analogWrite(MOTOR_PIN, CurrentDuty);
	}

	if (!WiFi.isConnected()) return;

	if (!isConnected) {
		if (LastDiscoverTime + DISCOVERY_TIMEOUT < millis()) {

			Logger.info("Searching For Haptic Server.");

			LastDiscoverTime = millis();
			send_discovery();
		}
	}
	
	if (isConnected && LastPacketTime + TIMEOUT < millis()) {

		Logger.info("Haptic Server Disconnected.");

		Address = IPAddress(255,255,255,255);
		isConnected = false;
		return;
	}

	if (UDP.parsePacket() == 0) return;
	UDP.read(Buffer ,sizeof(Buffer));
	
	switch (Buffer[3]) {
		case UINT8(PacketType::Handshake): {

			HandshakePacket handshake = UNPACK<HandshakePacket>(Buffer + 4);

			if (UNPACK<int>(handshake.secret) == 80085) {

				Logger.info("Haptic Server Connected!");

				LastPacketTime = millis();

				Address = UDP.remoteIP();
				isConnected = true;
			}

			break;
		}
		case UINT8(PacketType::Ping): {

			LastPacketTime = millis();
			send_ping();

			break;
		}
		case UINT8(PacketType::UpdateData): {

			UpdateDataPacket data = UNPACK<UpdateDataPacket>(Buffer + 4);
			
			if (!Testing) {

				LastDutyTime = millis();

				if (isInvertedPin) CurrentDuty = 255 - (int)data.duty;
				else CurrentDuty = (int)data.duty;;
			}

			break;
		}
	}
}

void Haptic::serial() {

	Logger.info("Testing Haptic...");
	Testing = true;

	LastDutyTime = 3000 + millis();

	if (isInvertedPin) CurrentDuty = 0;
	else CurrentDuty = 255;
}

