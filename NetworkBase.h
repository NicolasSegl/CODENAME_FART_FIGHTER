#pragma once

#include "Packet.h"
#include <enet/enet.h>

class NetworkBase
{
protected:
	unsigned short m_port;
	ENetAddress m_serverIPAddress;

	virtual void receiveData() = 0;
	virtual void sendData() = 0;
	
public:
	NetworkBase();

	virtual void update();
};