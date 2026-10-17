#include "haptic.h"

void Haptic::send(uint8_t type) {
	udp.beginPacket(ip,port);
	udp.write(0);
	udp.write(0);
	udp.write(0);
	udp.write(type);
	udp.endPacket();
}

void Haptic::setup() {
	udp.begin(port);
	if (!has_pin) {
		pinMode(MOTOR_PIN,OUTPUT);
	}
}

void Haptic::update() {

	if (!is_connected) {
		if (last_discovery + DISCOVERY_TIMEOUT < millis()) {
			logger.info("Searching For Haptic Server.");
			last_discovery = millis();
			Haptic::send((uint8_t)HapticPacketType::Discovery);
		}
	}

	if (is_connected && last_packet + TIMEOUT < millis()) {
		logger.info("Disconnected From Haptic Server.");
		is_connected = false;
		ip = IPAddress(255,255,255,255);
		return;
	}

	int size = udp.parsePacket();
	if (!size) {
		return;
	}
	(void)size;

	int length = udp.read(buffer,sizeof(buffer));

	last_packet = millis();

	switch (buffer[3]) {
		case (uint8_t)HapticPacketType::HandShake:
			logger.info("Connected To Haptic Server!");
			is_connected = true;
			ip = udp.remoteIP();
			break;
		case (uint8_t)HapticPacketType::Ping:
			Haptic::send((uint8_t)HapticPacketType::Pong);
			break;
		case (uint8_t)HapticPacketType::Data:

			DataPacket data;
			memcpy(&data, buffer + 4, sizeof(DataPacket));

			float vibration = resolve_field<float>(data.vibration);
			int duration = resolve_field<int>(data.duration);
			bool punch = data.punch;

#ifdef HAPTIC_DEBUG
			logger.debug("Recived Haptic Data: %f, %i, %s", vibration, duration, punch ? "true" : "false");
#endif

			if (has_pin) {

				//TODO pin controll
			}

			(void)data;

			(void)vibration;
			(void)duration;
			(void)punch;

			break;
	}

	(void)length;
}

void Haptic::test(SlimeVR::Logging::Logger &callback_logger) {
	logger.warn("NOT IMPLEMENTED YET!\n");
	callback_logger.info("Done.");
}