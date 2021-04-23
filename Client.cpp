#include "Client.h"
#include <iostream>
#include <thread>

Client::Client()
	: NetworkBase()
{
	init();
	m_clientHost = enet_host_create(nullptr, 1, 2, 0, 0);
	if (m_clientHost == nullptr)
		std::cout << "Error creating ENet client\n";
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

void Client::receiveData()
{
	ENetEvent event;
	while (enet_host_service(m_clientHost, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			Packet* packetReceived = (Packet*)(event.packet->data);
			if (packetReceived->packetRequest == PacketRequest::IDSendback)
			{
				std::cout << "id sent back from server\nid was: " << packetReceived->clientID << std::endl;
				// this tells the server what the id of the client sending packets is, 
				// and whether or not they have successfully connected to the server
				connected = true;
				id = packetReceived->clientID;

				// send a packet back to the server acknowledging that the id was successfully sent back
				Packet packet;
				packet.packetRequest == PacketRequest::SucessfulIDSendback;
				packet.clientID = id;
				
				ENetPacket* enetPacket = enet_packet_create(&packet, sizeof(packet), ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(m_peer, 0, enetPacket);
			}
		}
	}
}

void Client::sendData()
{
	// create and fill the packet with data
	Packet sendPacket;
	sendPacket.position = { (int)character->m_sprite.getPosition().x, (int)character->m_sprite.getPosition().y };
	sendPacket.packetRequest = PacketRequest::UpdateCharacter;
	sendPacket.clientID = id;

	// create an ENetPacket with the packet's data and send it to the server
	ENetPacket* enetPacket = enet_packet_create(&sendPacket, sizeof(sendPacket), 0); // a value of 0 means the packet is unreliable
	enet_peer_send(m_peer, 0, enetPacket);

	m_packetSent = true;
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
	Packet packet;
	m_peer = enet_host_connect(m_clientHost, &m_serverIPAddress, 2, 0);
	ENetEvent event;
	if (enet_host_service(m_clientHost, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		std::cout << "connection succeeded\n";
		enet_host_flush(m_clientHost);
	}
	else
		std::cout << "connection unsucessful\n";

	/*while (true)
	{
		//m_sendPacket.packetRequest = PacketRequest::ServerJoin;
		//m_sendPacket.

		//if (m_sendSocket.send(&m_sendPacket, sizeof(m_sendPacket), m_serverIP, m_serverPort) != sf::Socket::Done)
		//	std::cout << "Failed to connect to server\n";

		if (m_receiveSocket.receive(&packet, sizeof(packet), size, address, m_clientPort) != sf::Socket::Done)
		{
			// if it's not an id request, keep looping this
			std::cout << size << std::endl;
			if (packet.packetRequest == PacketRequest::IDSendback)
			{
				std::cout << "id: " << id << std::endl;
				id = packet.clientID;
				Packet sendPacket;
				sendPacket.packetRequest == PacketRequest::SucessfulIDSendback;
				if (m_sendSocket.send(&sendPacket, sizeof(sendPacket), m_serverIP, m_serverPort) != sf::Socket::Done)
					std::cout << "failed to send connection confirmation to server\n";
				else
					break;
			}
		}
	}*/
}

void Client::update()
{
	receiveData();
	sendData();
	character->update();
}