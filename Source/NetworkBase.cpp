#include "NetworkBase.h"
#include <iostream>
#include <string>

NetworkBase::NetworkBase()
{
	m_port = 6969;
	m_serverIPAddress.port = m_port;
}

void NetworkBase::update()
{
	receiveData();
	sendData();
}