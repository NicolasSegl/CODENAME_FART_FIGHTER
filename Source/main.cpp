#include "App.h"
#include <iostream>
#include <enet/enet.h>
#include <filesystem>

// when the client accepts a new character from the server, make sure that it doesn't already exist in the characterList
// RECEIVE PACKETS ON CLIENT ON ANOTHER THREAD
// beef rod hitboxes
// see client disconnect
// if you fall off pull em back up
// check the +/- 5 in character collision
// mulitple threads for server
// time based not frame based
// client update gets its own thread, server sending and receiving data are different threads (may have to use mutexs)
// when the server shutsdown, clients can reconnect. make it so the client knows if the server timed out
// proper server shutdown
// try making the client receive packets on another thread 
// can only change size of window in settings. full screen option too
// make server only send data when it needs to (if player moved, or had any other change of state)
// when adding animations and such, mind Character::hasChangedState()

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