#include "NetworkBase.h"
#include <iostream>

NetworkBase::NetworkBase()
{
	m_port = 6969;
	m_serverIPAddress.port = m_port;

	// this is temporary, just for testing
	// for now, this will do
	enet_address_set_host(&m_serverIPAddress, "10.0.0.135");
}

void NetworkBase::update()
{
	receiveData();
	sendData();
}