#include "App.h"
#include <iostream>
#include <enet/enet.h>

// maybe just a thread for every client for the server? so 5 clients connected to a server would result in the server using 5 threads to send the data
// ctrl-f new, then clean up memory leaks

// maybe make a function and pass a parameter an enum that will tell what data to send/receive?
// change client init to see if the ip is already in

// make more packets static. this may be faster?
// make separate folders for different Packet types (packet classes)
// make it so that, when a client disconnects, the server will remove the character from Client::serverCharacters
// the couts may be what is causing the lag on the server end. use mutexs to remedy this
// properly disconnect when the window closes
// fiddle with mutexs

// make everything safe, check for failures
// when the client accepts a new character from the server, make sure that it doesn't already exist in the characterList
// make it so that the server can connect to itself with a client of its own
// clean more shit up. in the packet class, make an actual destructor
// currently sending all packets as reliable. this may be slower, men adk

// BECOME COMMENT MAN. FUNCTIONS HAVE DESCRIPTIONS ABOVE THEM, THEIR PURPOSE. AAAAAAAAAAAAAAAA
// RECEIVE PACKETS ON CLIENT ON ANOTHER THREAD
// properly shut down client and server

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

	int x = 0;

	return 0;
}