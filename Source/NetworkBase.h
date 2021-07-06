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
	
public:
	NetworkBase();
	~NetworkBase();

	virtual void update();
	virtual void serviceHost() = 0;
	virtual void sendData() = 0;

	Level* getLevel() { return m_level; }
};