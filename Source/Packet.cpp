#include "Packet.h"
#include <iostream>

Packet::Packet()
{
}

ENetPacket* Packet::createEnetPacket(UDP reliability)
{
	static ENetPacket* enetPacket;

	if (reliability == UDP::RELIABLE)
		enetPacket = enet_packet_create(this, getSize(), ENET_PACKET_FLAG_RELIABLE);
	else
		enetPacket = enet_packet_create(this, getSize(), 0);

	return enetPacket;
}

void Packet::sendToPeer(ENetPeer* peer, UDP reliability)
{
	enet_peer_send(peer, 0, createEnetPacket(reliability));
}

void Packet::sendToAllPeers(ENetHost* host, UDP reliability)
{
	enet_host_broadcast(host, 0, createEnetPacket(reliability));
}