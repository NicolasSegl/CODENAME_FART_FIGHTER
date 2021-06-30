#pragma once

#include "Client.h"
#include "level.h"

#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>

const int PLAYER_CAP = 8;

class Server : public NetworkBase
{
private:
	std::vector<Client*> m_clients;
	int m_latestID = 0;

	ENetHost* m_serverHost;
	std::thread m_hostThread;

	virtual void sendData();
	virtual void receiveData();

	bool clientInit();

	void sendIDToClient(ENetPeer* peer);
	void updateClientCharacterList();
	void updateEntity(Packet* packet);
	void clientDisconnect(Packet* packet);
	void receiveAcknowledgement(Packet* packet, ENetPeer* peer);

public:
	Server();
	~Server();

	bool isHosting;

	virtual void update();
	void start(std::string ip);
	void handleClients();
	void renderClients(sf::RenderWindow& window);

	void shutdown();
};