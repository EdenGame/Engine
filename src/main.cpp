#include <iostream>
#include <SDL2/SDL.h>

#include "config.h"
#include "client/client.hpp"

void shutdown(void) {
	std::cout << "Shutdown." << std::endl;
}

int main() {
	std::cout << "Welcome to " << CONFIG_PROJECT_NAME << " version " << CONFIG_VERSION_STRING << std::endl;

	// Assign atexit handler
	if (std::atexit(shutdown)) {
		std::cout << "Failed to assign exit handler, aborting." << std::endl;
		return -1;
	}

	// Create (and initialize where applicable) singletons

	MClient::Client *client = MClient::Client::getInstance();

	// Run client main loop
	client->main();

	return 0;
}
