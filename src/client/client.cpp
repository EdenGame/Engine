#include "client.hpp"

using namespace MClient;

/* Client Class */

// Constructor
Client::Client() {
	std::cout << "Initializing client..." << std::endl;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		std::exit(1);
	}

	// Request forward-compatible OpenGL 3.3 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Create window
	window = MClient::Window("ProjectEden");

	// Initialize GLAD
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		std::exit(1);
	}

	MClient::windowSize size = window.getSize();
	glViewport(0, 0, size.w, size.h);

	// Enable Depth Buffer
	glEnable(GL_DEPTH_TEST);
}

Client::~Client() {}

// [Client] Get singleton
Client *Client::getInstance() {
	static Client instance;

	return &instance;
}

// [void] Main event loop
void Client::main() {
	SDL_Event event;
	bool shouldQuit = false;
	while (!shouldQuit) {
		// Handle Events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					glViewport(0, 0, event.window.data1, event.window.data2);
				}
			} else if (event.type == SDL_QUIT) {
				shouldQuit = true;
			}
		}
	}
}
