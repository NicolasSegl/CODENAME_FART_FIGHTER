#pragma once

#include "Packet.h"
#include "Level.h"

#include <enet/enet.h>

class NetworkBase
{
protected:
	unsigned short m_port;
	ENetAddress m_serverIPAddress;
	Level* m_level;

	virtual void receiveData() = 0;
	virtual void sendData() = 0;
	
public:
	NetworkBase();
	~NetworkBase();

	virtual void update();

	Level* getLevel() { return m_level; }
};