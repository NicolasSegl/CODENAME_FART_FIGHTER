#pragma once

#include "utils.h"
#include "Character.h"
#include <enet/enet.h>

enum class PacketRequest
{
	AcknowledgeID,
	SendID,
	EntityUpdate,
	ClientDisconnect,
	EntityListChange,
	ServerShutdown,
	LoadLevel,
};

enum class UDP
{
	RELIABLE,
	UNRELIABLE,
};

struct Packet
{
	PacketRequest packetRequest;
	unsigned short clientID;

	Packet();

	void sendToPeer(ENetPeer* peer, UDP reliability);
	void sendToAllPeers(ENetHost* host, UDP reliability);

	virtual int getSize() { return sizeof(*this); }
};

struct EntityUpdatePacket : public Packet
{
	vec2i position;
	int getSize() { return sizeof(*this); }
};

struct NewEntityPacket : public Packet
{
	Character newCharacter;
	bool connected = true;
	int getSize() { return sizeof(*this); }
};

struct LevelPacket : public Packet
{
	char levelName[64];
	int getSize() { return sizeof(*this); }
};