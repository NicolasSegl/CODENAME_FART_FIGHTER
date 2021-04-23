#pragma once

#include "Character.h"
#include "NetworkBase.h"
#include <string>

class Client : public NetworkBase
{
private:

	ENetPeer* m_peer;
	ENetHost* m_clientHost;

	virtual void sendData();
	virtual void receiveData();

	void receiveIDFromServer();

public:
	int id;
	Character* character;
	bool connected = false;

	Client();

	bool connectToIp(const std::string& ip);
	void init();
	void serverCreationInit();

	virtual void update();
};