enum class HapticPacketType : uint8_t {
	Discovery = 1,
	HandShake = 2,
	Ping = 3,
	Pong = 4,
	Data = 5,
};

struct DataPacket {
	uint8_t vibration[4];
	uint8_t duration[4];
	bool punch;
};

template <typename T>
T resolve_field(uint8_t *bytes) {
	T value;
	memcpy(&value, bytes, sizeof(T));
	return value;
}