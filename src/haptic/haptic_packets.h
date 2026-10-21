enum PacketType {
	Discovery = 1,
	Handshake = 2,
	Ping = 3,
	UpdateData = 4,
};

struct UpdateDataPacket {
	uint8_t duty;
};

struct DiscoveryPacket {
	uint8_t mac[6];
};

struct HandshakePacket {
	uint8_t secret[4];
};