#pragma once

#include <string>

enum class MenuResponse
{
	ConnectToIp,
	HostOnIp,
	NoResponse
};

struct MenuResponseData
{
	MenuResponse response;
};

struct IPResponse : public MenuResponseData
{
	char ip[32];
};