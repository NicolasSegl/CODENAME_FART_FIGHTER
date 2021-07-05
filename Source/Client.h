#pragma once

#include "Character.h"
#include "NetworkBase.h"
#include "Level.h"

#include <string>
#include <vector>

class Client : public NetworkBase
{
private:

	ENetPeer* m_peer;
	ENetHost* m_clientHost;
	std::vector<Character> m_serverCharacters;

	void receiveID();
	void receiveNewEntity(NewEntityPacket* packetReceived);

	void sendID(Packet* packetReceived);
	void updateEntity();

public:
	int id;
	Character* character;
	bool connected = false;

	Client();
	~Client();

	bool connectToIp(const std::string& ip);
	void init();
	void serverCreationInit();
	void disconnect();
	void updateCharacter();

	std::vector<Character>& getServerCharacters() { return m_serverCharacters; }

	virtual void sendData();
	virtual void receiveData();
	virtual void update();
};