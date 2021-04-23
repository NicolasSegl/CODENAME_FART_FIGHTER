#include "Server.h"
#include <iostream>
#include <thread>
#include <mutex>

Server::Server()
	: NetworkBase()
{
	m_serverIPAddress.host = ENET_HOST_ANY;
	m_serverHost = enet_host_create(&m_serverIPAddress, 8, 2, 0, 0);
	if (m_serverHost == nullptr)
	{
		std::cout << "Error creating ENet server host\n";
	}
}

void Server::sendData()
{
	/*char data[] = "data";

	// CHANGE SERVER_IP TO CLIENT'S IP THAT DATA IS BEING SENT TO
	if (m_sendSocket.send(data, sizeof(data), m_serverIP, m_clientPort) != sf::Socket::Done)
		std::cout << "failed to send\n";

	m_packetSent = true;*/
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

	m_clients[m_latestID] = new Client;
	m_clients[m_latestID]->serverCreationInit();
	m_clients[m_latestID]->id = m_latestID;

	m_latestID++;
	return true;
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
			std::cout << "Connection detected\n";

			// if the client is successfully created
			if (clientInit())
			{
				Packet packet;
				packet.packetRequest = PacketRequest::IDSendback;
				packet.clientID = m_latestID - 1;

				// check if packet is successfully created
				ENetPacket* enetPacket = enet_packet_create(&packet, sizeof(packet), ENET_PACKET_FLAG_RELIABLE);
				// sends to the same peer that connected...
				enet_peer_send(event.peer, 0, enetPacket);
			}
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			Packet* packetReceived = (Packet*)(event.packet->data);

			if (!m_clients[packetReceived->clientID]->connected)
			{
				if (packetReceived->packetRequest == PacketRequest::SucessfulIDSendback)
				{
					m_clients[packetReceived->clientID]->connected = true;
					break;
				}
			}
			else
			{
				switch (packetReceived->packetRequest)
				{
				case PacketRequest::UpdateCharacter:
					m_clients[packetReceived->clientID]->character->m_sprite.setPosition(sf::Vector2f(packetReceived->position.x, packetReceived->position.y));
					break;
				}
			}
		}
	}
}

void Server::start()
{
	// create a thread that will send/receive data for the server.
	static std::thread hostThread(&Server::handleClients, this);
}

void Server::handleClients()
{
	while (true)
		update();
}

void Server::update()
{
	receiveData();
	//sendData();
}

void Server::renderClients(sf::RenderWindow& window)
{
	for (auto& client : m_clients)
	{
		if (client)
		{
			window.draw(client->character->m_sprite);
		}
	}
}