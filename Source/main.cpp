#include "App.h"
#include <iostream>
#include <enet/enet.h>
#include <filesystem>

// maybe just a thread for every client for the server? so 5 clients connected to a server would result in the server using 5 threads to send the data
// ctrl-f new, then clean up memory leaks
// maybe make a function and pass a parameter an enum that will tell what data to send/receive?
// change client init to see if the ip is already in
// the couts may be what is causing the lag on the server end. use mutexs to remedy this

// make everything safe, check for failures
// when the client accepts a new character from the server, make sure that it doesn't already exist in the characterList
// make it so that the server can connect to itself with a client of its own
// clean more shit up. in the packet class, make an actual destructor

// BECOME COMMENT MAN. FUNCTIONS HAVE DESCRIPTIONS ABOVE THEM, THEIR PURPOSE. AAAAAAAAAAAAAAAA
// RECEIVE PACKETS ON CLIENT ON ANOTHER THREAD
// beef rod hitboxes
// see client disconnect
// mutex
// if you fall off pull em back up
// give vector structs += and -= operators
// check the +/- 5 in character collision
// basic menu
// mulitple threads for server
// levels with images

// client update gets its own thread, server sending and receiving data are different threads (may have to use mutexs)

int main()
{
	CURRENT_WORKING_DIRECTORY = std::filesystem::current_path().string();

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