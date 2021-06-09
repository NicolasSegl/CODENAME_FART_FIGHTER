#include "Client.h"
#include <iostream>
#include <mutex>

Client::Client()
	: NetworkBase()
{
	id = -1;
	init();
	m_clientHost = enet_host_create(nullptr, 1, 1, 0, 0);
	if (m_clientHost == nullptr)
		std::cout << "Error creating ENet client\n";
}

void Client::disconnect()
{
	Packet packet;
	packet.packetRequest = PacketRequest::ClientDisconnect;
	//packet.sendToPeer(m_peer);
}

Client::~Client()
{
	disconnect();
	enet_host_destroy(m_clientHost);
	enet_peer_reset(m_peer);
}

void Client::init()
{
	character = new Character;
	character->init();
}

void Client::serverCreationInit()
{
	character->init();
}

void Client::sendIDBackToServer(Packet* packetReceived)
{
	// this tells the server what the id of the client sending packets is, 
	id = packetReceived->clientID;

	// send a packet back to the server acknowledging that the id was successfully sent back
	Packet packet;
	packet.packetRequest = PacketRequest::AcknowledgeID;
	packet.clientID = id;
	packet.sendToPeer(m_peer, UDP::RELIABLE);
	m_connected = true;
}

void Client::receiveData()
{
	ENetEvent event;
	while (enet_host_service(m_clientHost, &event, 0) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				Packet* packetReceived = (Packet*)(event.packet->data);
				switch (packetReceived->packetRequest)
				{
					case PacketRequest::SendID:
					{
						// give our client's character the same id
						character->id = id;
						sendIDBackToServer(packetReceived);
						break;
					}
					case PacketRequest::EntityListChange:
					{
						NewEntityPacket* receivedPacket = (NewEntityPacket*)(event.packet->data);

						// checking to see if we need to add a new character to this client's array (we shouldn't add any twice)
						if (receivedPacket->clientID == id)
							break;
						for (auto& character : m_serverCharacters)
							if (character.id == receivedPacket->clientID )
								goto breakLabel;

						m_serverCharacters.push_back(receivedPacket->newCharacter);
						m_serverCharacters[m_serverCharacters.size() - 1].init();

						breakLabel:
						break;
					}
					case PacketRequest::EntityUpdate:
					{
						EntityUpdatePacket* receivedPacket = (EntityUpdatePacket*)(event.packet->data);
						if (receivedPacket->clientID != id && receivedPacket->clientID < m_serverCharacters.size())
							m_serverCharacters[receivedPacket->clientID].updateFromServer(receivedPacket);

						break;
					}
					default:
						break;
				}

				enet_packet_destroy(event.packet);
			}
		}
	}
}

void Client::updateEntity()
{
	static EntityUpdatePacket sendPacket;
	sendPacket.position = { (int)character->sprite.getPosition().x, (int)character->sprite.getPosition().y };
	sendPacket.packetRequest = PacketRequest::EntityUpdate;
	sendPacket.clientID = id;
	sendPacket.sendToPeer(m_peer, UDP::RELIABLE); // should this be reliable?
}

void Client::sendData()
{
	updateEntity();
}

bool Client::connectToIp(const std::string& ip)
{
	// tell it to wait for a response. 
	enet_address_set_host(&m_serverIPAddress, ip.c_str());
	m_serverIPAddress.port = m_port;
	receiveIDFromServer();
	return true;
}

void Client::receiveIDFromServer()
{
	m_peer = enet_host_connect(m_clientHost, &m_serverIPAddress, 1, 0);
	if (m_peer == NULL)
	{
		std::cout << "No available peers for inititating an ENet connection\n";
		exit(0);
	}

	ENetEvent event;
	if (enet_host_service(m_clientHost, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		std::cout << "Connection successful\n";
	else
		std::cout << "connection unsucessful\n";
}

void Client::update()
{
	receiveData();
	if (m_connected)
		sendData();
	character->update();
}