#include "App.h"
#include <iostream>
#include <enet/enet.h>

// multithread her
// make resizable
// make separate exe for server?
// the client/server update functions are looking very similar...
// maybe just a thread for every client for the server? so 5 clients connected to a server would result in the server using 5 threads to send the data
// ctrl-f new, then clean up memory leaks
// close threads when window closes
// readd the output statements, only use mutex to protect the couts
// maybe remove m_receivePacket
// add/remove couts. cout for having a player connect, etc 

// maybe make std::size_t size and sf::IpAddress address members of NetworkBase?
// maybe make a function and pass a parameter an enum that will tell what data to send/receive?
// make window resizable, and have everything actually work with that. good luck with that shit fam
// JESUS CLEAN IT UP OH NONONONONO
// when the server sends data, make it use enet_host_broadcast, not enet_peer_send
// make a function (or something equivalent) in Packet that will return an ENetPacket* that holds the same data as the Packet

int main()
{
	// initialize ENet
	if (enet_initialize() != 0)
	{
		std::cout << "An error ocurred while initializing Enet\n";
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	App app;
	app.run();

	return 0;
}