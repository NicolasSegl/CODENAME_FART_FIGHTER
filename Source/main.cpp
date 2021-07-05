#include "App.h"
#include <iostream>
#include <enet/enet.h>
#include <filesystem>

// ctrl-f new, then clean up memory leaks
// make everything safe, check for failures
// when the client accepts a new character from the server, make sure that it doesn't already exist in the characterList
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
// time based not frame based
// client update gets its own thread, server sending and receiving data are different threads (may have to use mutexs)
// when the server shutsdown, clients can reconnect

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