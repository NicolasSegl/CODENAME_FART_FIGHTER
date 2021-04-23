#pragma once

#include "Client.h"
#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>

const int PLAYER_CAP = 16;

class Server : public NetworkBase
{
private:
	Client* m_clients[PLAYER_CAP];
	int m_latestID = 0;
	//std::vector<std::thread> m_clientThreads; 

	ENetHost* m_serverHost;

	virtual void sendData();
	virtual void receiveData();

	bool clientInit();

public:
	Server();

	virtual void update();
	void start();
	void handleClients();
	void renderClients(sf::RenderWindow& window);
};