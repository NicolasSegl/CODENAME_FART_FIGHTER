#include "App.h"
#include <iostream>
#include <enet/enet.h>

// make resizable
// make separate exe for server?
// maybe just a thread for every client for the server? so 5 clients connected to a server would result in the server using 5 threads to send the data
// ctrl-f new, then clean up memory leaks

// maybe make a function and pass a parameter an enum that will tell what data to send/receive?
// change client init to see if the ip is already in

// send characters to clients. then simply update them (their position, namely)
// make more packets static. this may be faster?
// make separate folders for different Packet types (packet classes)
// make it so that, when a client disconnects, the server will remove the character from Client::serverCharacters
// the couts may be what is causing the lag on the server end. use mutexs to remedy this
// properly disconnect when the window closes
// make separate functions for every packet sent/received
// problems with lag when two exe's are launched, and not just one and the debugger
// fiddle with mutexs

// make everything safe, check for failures
// please jesus

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