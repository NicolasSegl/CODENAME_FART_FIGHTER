#pragma once

#include "utils.h"

enum class PacketRequest
{
	SucessfulIDSendback,
	IDSendback,
	UpdateCharacter,
};

class Packet
{
private:
	char* m_rawData;

public:
	PacketRequest packetRequest;
	unsigned short clientID;
	vec2i position;

	Packet();
};