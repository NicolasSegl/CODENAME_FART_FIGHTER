#include "Server.h"
#include <iostream>
#include <thread>
#include <mutex>

Server::Server()
	: NetworkBase()
{
	isHosting = false;
}

void Server::shutdown()
{
	if (isHosting)
		m_hostThread.join();
}

Server::~Server()
{
	shutdown();
	enet_host_destroy(m_serverHost);
}

std::mutex mutex;
void Server::sendData()
{
	//mutex.lock();

	//std::cout << "sending data from client: " << std::endl;
	for (auto& client : m_clients)
	{
		static EntityUpdatePacket packet;
		packet.packetRequest = PacketRequest::EntityUpdate;
		packet.clientID = client->id;
		packet.position = client->character->getPos();
		packet.sendToAllPeers(m_serverHost, false);
	}

	//mutex.unlock();
}

void Server::sendIDToClient(ENetPeer* peer)
{
	static Packet packet;
	packet.clientID = m_latestID;
	packet.packetRequest = PacketRequest::SendID;
	packet.sendToPeer(peer, true);
}

bool Server::clientInit()
{
	if (m_latestID >= PLAYER_CAP)
		return false;
	/*for (int client = 0; client < PLAYER_CAP; client++)
		if (!m_clients[m_latestID])
			for (int i = 0; i < PLAYER_CAP; i++)
				if (m_clients[i]->clientIP == receivedPacket.senderIP)
					return;
	*/

	// create a new client and initialize it with default values
	m_clients.push_back(new Client);
	m_clients[m_clients.size() - 1]->serverCreationInit();
	m_clients[m_clients.size() - 1]->id = m_latestID;
	m_latestID++;

	return true;
}

void Server::updateClientCharacterList()
{
	static NewEntityPacket packet;
	packet.packetRequest = PacketRequest::EntityListChange;
	packet.newCharacter = *(m_clients[m_clients.size() - 1]->character);
	packet.sendToAllPeers(m_serverHost, true);
}

void Server::updateEntity(Packet* packet)
{
	EntityUpdatePacket* packetReceived = (EntityUpdatePacket*)(packet);

	// update the position of the sprite to be drawn
	m_clients[packetReceived->clientID]->character->setPos(packetReceived->position);
	m_clients[packetReceived->clientID]->character->sprite.setPosition(sf::Vector2f
	(
		packetReceived->position.x,
		packetReceived->position.y
	));
}

void Server::receiveData()
{
	// iterate through all the packets received by the server
	ENetEvent event;
	while (enet_host_service(m_serverHost, &event, 0) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
			{
				std::cout << "A client has connected from: " << &event.peer->address.host << std::endl;
				// sends to the same peer that connected...
				sendIDToClient(event.peer);
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				Packet* packetReceived = (Packet*)(event.packet->data);

				if (packetReceived->packetRequest == PacketRequest::AcknowledgeID)
				{
					if (clientInit())
					{
						std::cout << "Client " << packetReceived->clientID << " successfully initialized\n";
						updateClientCharacterList();
					}
					else
						std::cout << "Client failed to initialize\n";
					break;
				}
				if (m_clients[packetReceived->clientID])
				{
					switch (packetReceived->packetRequest)
					{
						case PacketRequest::EntityUpdate:
						{
							updateEntity(packetReceived);
							break;
						}
						case PacketRequest::ClientDisconnect:
						{
							clientDisconnect(packetReceived);
							break;
						}
					}
				}

				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				break;
			}
		}
	}
}

void Server::clientDisconnect(Packet* packet)
{
	std::cout << "Client " << packet->clientID << "has disconnected\n";
	delete m_clients[packet->clientID];
	m_clients[packet->clientID] = nullptr;
}

void Server::start(std::string ip)
{
	enet_address_set_host(&m_serverIPAddress, ip.c_str());

	m_serverIPAddress.host = ENET_HOST_ANY;
	m_serverHost = enet_host_create(&m_serverIPAddress, PLAYER_CAP, 2, 0, 0);
	if (m_serverHost == nullptr)
		std::cout << "Error creating ENet server host\n";
	// create a thread that will send/receive data for the server.
	m_hostThread = std::thread(&Server::handleClients, this);
}

void Server::handleClients()
{
	while (true)
		update();
}

void Server::update()
{
	mutex.lock();
	receiveData();
	sendData();
	mutex.unlock();
}

void Server::renderClients(sf::RenderWindow& window)
{
	for (auto& client : m_clients)
	{
		if (client)
		{
			window.draw(client->character->sprite);
		}
	}
}