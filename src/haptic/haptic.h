#include <Arduino.h>
#include <WiFiUdp.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include "haptic_defines.h"
#include "haptic_packets.h"

#include "../logging/Logger.h"

//#include "quat.h"
//#include "vector3.h"

#define UINT8(value) (uint8_t)value

template <typename Type>
Type UNPACK(uint8_t *bytes) {
	Type value;
	memcpy(&value, bytes, sizeof(Type));
	return value;
}

class Haptic {

	public:

	static Haptic& Instance() {
		static Haptic intance;
		return intance;
	}

	void setup();
	void update();
	void serial();

	private:

	void begin_packet();

	template <typename Packet = PacketType>
	void send(PacketType type, Packet packet);

	void send_ping();
	void send_discovery();

	Haptic() {}
	~Haptic() {}

	SlimeVR::Logging::Logger Logger = SlimeVR::Logging::Logger("HAPTIC");

	WiFiUDP UDP;
	unsigned char Buffer[128];

	IPAddress Address = IPAddress(255,255,255,255);
	const int Port = 6767;
	
	bool isConnected = false;

	bool hasPin = (MOTOR_PIN != -1);
	bool isInvertedPin = (MOTOR_PIN_INVERTED == true);

	const int DUTY_ZERO = isInvertedPin ? 255 : 0;

	int CurrentDuty = DUTY_ZERO;

	unsigned long LastDutyTime;
	unsigned long LastDiscoverTime;
	unsigned long LastPacketTime;

	bool Testing = false;

};

