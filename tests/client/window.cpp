#include <catch2/catch.hpp>
#include <iostream>

#include <client/window.hpp>

using namespace MClient;

TEST_CASE("windows can be created and mutated", "[window]") {
	Window *window = new Window;

	SECTION("changing the title affects the window") {
		std::string title = "New Title";
		window->setTitle(title);
		REQUIRE(window->getTitle() == title);
		REQUIRE(SDL_GetWindowTitle(window->getWindow()) == title);
	}

	SECTION("repositioning changes the position of the object and window") {
		int nx = 10; // New pos x
		int ny = 20; // New pos y
		window->setPos(nx, ny); // Set new position
		windowAttr pos = window->getPos(); // Get position struct
		REQUIRE(nx == pos.x);
		REQUIRE(ny == pos.y);
		// Get postion via SDL API
		int x = 0;
		int y = 0;
		SDL_GetWindowPosition(window->getWindow(), &x, &y);
		REQUIRE(nx == x);
		REQUIRE(ny == y);
	}

	SECTION("resizing changes the size of the object and window") {
		int nw = 100; // New width
		int nh = 581; // New height
		window->setSize(nw, nh); // Set new size
		windowAttr size = window->getSize(); // Get size struct
		REQUIRE(nw == size.x);
		REQUIRE(nh == size.y);
		// Get postion via SDL API
		int w = 0;
		int h = 0;
		SDL_GetWindowSize(window->getWindow(), &w, &h);
		REQUIRE(nw == w);
		REQUIRE(nh == h);
	}

	SECTION("showing the window updates the SDL flags") {
		window->show();
		REQUIRE_FALSE(window->isHidden());
		REQUIRE((SDL_GetWindowFlags(window->getWindow()) & SDL_WINDOW_SHOWN));
	}

	SECTION("hiding the window updates the SDL flags") {
		window->hide();
		REQUIRE(window->isHidden());
		REQUIRE((SDL_GetWindowFlags(window->getWindow()) & SDL_WINDOW_HIDDEN));
	}

	SECTION("calling the use context method changes the global GL context") {
		window->useContext();
		REQUIRE(SDL_GL_GetCurrentContext() == *window->getContext());
	}

	SECTION("deleting a window object destroys the window") {
		Uint32 id = SDL_GetWindowID(window->getWindow()); // Get window ID
		delete window; // Delete object
		REQUIRE_FALSE(SDL_GetWindowFromID(id)); // Attempt to fetch window from ID
	}

	SECTION("calling close destroys the window") {
		Window toclose = Window(); // Create new window
		Uint32 id = SDL_GetWindowID(toclose.getWindow()); // Get window ID
		toclose.close();
		REQUIRE_FALSE(SDL_GetWindowFromID(id)); // Attempt to fetch window from ID
	}
}
