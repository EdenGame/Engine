#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace MClient {

struct windowPos {
	int x = 0;
	int y = 0;
	windowPos();
	windowPos(int x, int y);
};

struct windowSize {
	int w = 1024;
	int h = 576;
	windowSize();
	windowSize(int w, int h);
};

class Window {
	public:
		Window();
		Window(std::string name); // With name
		Window(std::string name, windowPos pos); // With name and position struct
		Window(std::string name, float x, float y); // With name and position floats
		Window(std::string name, windowPos pos, windowSize size); // With name, position struct, and size struct
		Window(std::string name, float x, float y, float w, float h); // With name, position and size floats
		~Window();

		std::string getTitle();
		void setTitle(std::string name);

		windowPos getPos();
		void setPos(windowPos pos);
		void setPos(float x, float y);
		windowSize getSize();
		void setSize(windowSize size);
		void setSize(float w, float h);

		void hide();
		void show();
		void minimize();
		void maximize();
		void restore();
		void close();

		void makeContextCurrent();

		bool handleEvent(SDL_Event event);
		SDL_Window* getWindow();
	private:
		std::string name = "";
		SDL_Window* window;
		SDL_GLContext context;

		void init(float x, float y, float w, float h); // Initializer shared between many constructors
};

}
