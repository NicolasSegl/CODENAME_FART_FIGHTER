#include "Packet.h"
#include <iostream>

Packet::Packet()
{
}

void Packet::sendToPeer(ENetPeer* peer, bool reliable)
{
	ENetPacket* enetPacket;
	if (reliable)
		enetPacket = enet_packet_create(this, getSize(), ENET_PACKET_FLAG_RELIABLE);
	else
		enetPacket = enet_packet_create(this, getSize(), 0);
	enet_peer_send(peer, 0, enetPacket);
}

void Packet::sendToAllPeers(ENetHost* host, bool reliable)
{
	ENetPacket* enetPacket;
	if (reliable)
		enetPacket = enet_packet_create(this, getSize(), ENET_PACKET_FLAG_RELIABLE);
	else
		enetPacket = enet_packet_create(this, getSize(), 0);

	enet_host_broadcast(host, 0, enetPacket);
}