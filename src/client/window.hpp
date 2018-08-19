#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace MClient {

struct windowAttr {
	int x = 0;
	int y = 0;
	windowAttr();
	windowAttr(int x, int y);
};

class Window {
	public:
		Window();
		Window(std::string name); // With name
		~Window();

		std::string getTitle();
		void setTitle(std::string name);

		windowAttr getPos();
		void setPos(int x, int y);
		windowAttr getSize();
		void setSize(int w, int h);

		void hide();
		void show();
		void close();
		bool isHidden();

		void useContext();

		bool handleEvent(SDL_Event event);
		SDL_Window* getWindow();
		SDL_GLContext* getContext();
	private:
		std::string name = "";
		SDL_Window* window;
		SDL_GLContext context;
};

}
