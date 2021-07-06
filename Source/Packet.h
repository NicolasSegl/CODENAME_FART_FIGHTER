#pragma once

#include "utils.h"
#include "Character.h"
#include <enet/enet.h>

// The enumeration variable sent in every packet should be as small as possible. 
// As suchm it is only 8 bits. The same is done with the UDP enum class
enum class PacketRequest : uint8_t
{
	AcknowledgeID,
	SendID,
	EntityUpdate,
	ClientDisconnect,
	EntityListChange,
	ServerShutdown,
	LoadLevel,
};

enum class UDP : uint8_t
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

private:
	ENetPacket* createEnetPacket(UDP reliability);
};

struct EntityUpdatePacket : public Packet
{
	vec2i position;

	int getSize() { return sizeof(*this); }
	EntityUpdatePacket() { packetRequest = PacketRequest::EntityUpdate; }
};

struct NewEntityPacket : public Packet
{
	Character newCharacter;
	bool connected = true;

	int getSize() { return sizeof(*this); }
	NewEntityPacket() { packetRequest = PacketRequest::EntityListChange; }
};

struct LevelPacket : public Packet
{
	char levelName[64];

	int getSize() { return sizeof(*this); }
	LevelPacket() { packetRequest = PacketRequest::LoadLevel; }
};