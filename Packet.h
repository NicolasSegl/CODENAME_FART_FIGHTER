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
};

struct Packet
{
	PacketRequest packetRequest;
	unsigned short clientID;

	Packet();

	void sendToPeer(ENetPeer* peer, bool reliable);
	void sendToAllPeers(ENetHost* host, bool reliable);

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
	int getSize() { return sizeof(*this); }
};