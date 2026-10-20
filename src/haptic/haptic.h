#include <Arduino.h>
#include <WiFiUdp.h>

#include "haptic_defines.h"
#include "haptic_packets.h"

#include "../logging/Logger.h"

class Haptic {

	public:

	static Haptic& Instance() {
		static Haptic intance;
		return intance;
	}

	void setup();
	void update();
	void send(uint8_t type);
	void test(SlimeVR::Logging::Logger &callback_logger);

	private:
	
	Haptic() {}

	bool has_pin = MOTOR_PIN != -1;

	SlimeVR::Logging::Logger logger = SlimeVR::Logging::Logger("HAPTIC");
	WiFiUDP udp;

	IPAddress ip = IPAddress(255,255,255,255);
	int port = 6767;

	bool is_connected = false;

	unsigned char buffer[124];

	unsigned long last_packet;
	unsigned long last_discovery;

 unsigned long start_vibration;
};

