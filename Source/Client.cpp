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
	std::cout << "Disconnecting from server\n";

	Packet packet;
	packet.packetRequest = PacketRequest::ClientDisconnect;
	packet.clientID = id;
	packet.sendToPeer(m_peer, UDP::RELIABLE);

	// send the final packets
	enet_host_flush(m_clientHost);
	connected = false;
}

Client::~Client()
{
	if (connected)
		disconnect();
	enet_host_destroy(m_clientHost);
	enet_peer_reset(m_peer);
	delete character;
}

void Client::init()
{
	character = new Character;
	character->init(id);
}

void Client::serverCreationInit()
{
	character->init(id);
}

void Client::sendID(Packet* packetReceived)
{
	// this tells the server what the id of the client sending packets is, 
	id = packetReceived->clientID;

	// send a packet back to the server acknowledging that the id was successfully sent back
	Packet packet;
	packet.packetRequest = PacketRequest::AcknowledgeID;
	packet.clientID = id;
	packet.sendToPeer(m_peer, UDP::RELIABLE);;
	connected = true;
}

void Client::receiveNewEntity(NewEntityPacket* receivedPacket)
{
	// checking to see if we need to add a new character to this client's array (we shouldn't add any twice, nor add ourselves)
	for (auto& character : m_serverCharacters)
		if (character.id == receivedPacket->clientID)
			return;

	// afterwards so there is no out of bounds error
	if (id == receivedPacket->clientID)
	{
		// initiate the character in the server character list
		m_serverCharacters.push_back(*character);
		if (character)
			delete character;
		character = &m_serverCharacters[id];
		return;
	}

	m_serverCharacters.push_back(receivedPacket->newCharacter);
	m_serverCharacters[m_serverCharacters.size() - 1].init();

	// due to the memory allocation when an std::vector calls ::push_back(), this is necessary
	// otherwise, it will be pointing to some random point in memory (of which std::vector has deleted)
	if (m_serverCharacters.size() > id)
		character = &m_serverCharacters[id];
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
						// give our client's character the same id as our client
						sendID(packetReceived);
						character->id = id;
						break;
					}
					case PacketRequest::EntityListChange:
					{
						receiveNewEntity((NewEntityPacket*)(event.packet->data));
						break;
					}
					case PacketRequest::EntityUpdate:
					{
						EntityUpdatePacket* receivedPacket = (EntityUpdatePacket*)(event.packet->data);
						if (receivedPacket->clientID != id && receivedPacket->clientID < m_serverCharacters.size())
							m_serverCharacters[receivedPacket->clientID].updateFromServer(receivedPacket);

						break;
					}
					case PacketRequest::ServerShutdown:
					{
						std::cout << "The server has shutdown\n";
						m_serverCharacters.clear();
						// disconnect ?
						break;
					}
					case PacketRequest::ClientDisconnect:
					{
						std::cout << "A client has disconnected\n";
						// This will render the character dead, and make it as though the character isn't there at all.
						m_serverCharacters[packetReceived->clientID].isAlive = false;
						break;
					}
					case PacketRequest::LoadLevel:
					{
						LevelPacket* levelPacket = (LevelPacket*)(event.packet->data);
						m_level->loadLevel(levelPacket->levelName);
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
	sendPacket.sendToPeer(m_peer, UDP::RELIABLE);
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
	receiveID();
	return true;
}

void Client::receiveID()
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
	if (connected)
	{
		sendData();
		character->update(m_level->getPlatforms());
	}
}