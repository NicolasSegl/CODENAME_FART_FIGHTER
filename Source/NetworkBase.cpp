#include "NetworkBase.h"
#include <iostream>
#include <string>

NetworkBase::NetworkBase()
{
	m_port = 6969;
	m_serverIPAddress.port = m_port;
	m_level = new Level;
}

NetworkBase::~NetworkBase()
{
	delete m_level;
}

void NetworkBase::update()
{
	serviceHost();
	sendData();
}