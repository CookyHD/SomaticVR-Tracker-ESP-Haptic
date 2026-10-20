enum class HapticPacketType : uint8_t {
	Discovery = 1,
	HandShake = 2,
	Ping = 3,
	Pong = 4,
	DataVibration = 5,
 DataPunch = 6
};

struct DataPacketVibration {
	uint8_t vibration[4];
	uint8_t duration[4];
};

struct DataPacketPunch {
	uint8_t strength[4];
};

template <typename T>
T resolve_field(uint8_t *bytes) {
	T value;
	memcpy(&value, bytes, sizeof(T));
	return value;
}