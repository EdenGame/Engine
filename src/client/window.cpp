#include "window.hpp"

using namespace MClient;

/* Window Class */

windowAttr::windowAttr() {}
windowAttr::windowAttr(int x, int y): x(x), y(y) {}

// [constructor] Initialize blank window
Window::Window() {
	this->window = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

	// Create OpenGL Context
	this->context = SDL_GL_CreateContext(this->window);
	SDL_GL_MakeCurrent(this->window, this->context);

	// Initialize GLAD
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		std::exit(1);
	}
}
// [destructor] Ensure window is closed and context is destroyed
Window::~Window() {
	SDL_GL_DeleteContext(this->context);
	this->close();
}

// [string] Get window title
std::string Window::getTitle() {
	return this->name;
}
// [void] Set window title
void Window::setTitle(std::string name) {
	this->name = name;
	SDL_SetWindowTitle(this->window, name.c_str());
}

// [windowPos] Get window position
windowAttr Window::getPos() {
	int x = 0;
	int y = 0;
	SDL_GetWindowPosition(this->window, &x, &y);
	return windowAttr(x, y);
}
// [void] Set window position
void Window::setPos(int x, int y) {
	SDL_SetWindowPosition(this->window, x, y);
}

// [windowSize] Get window size
windowAttr Window::getSize() {
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(this->window, &w, &h);
	return windowAttr(w, h);
}
// [void] Set window size
void Window::setSize(int w, int h) {
	SDL_SetWindowSize(this->window, w, h);
	glViewport(0, 0, w, h);
}

// [void] Hide window
void Window::hide() {
	SDL_HideWindow(this->window);
}
// [void] Show window
void Window::show() {
	SDL_ShowWindow(this->window);
}
// [void] Close window
void Window::close() {
	SDL_DestroyWindow(this->window);
}
// [bool] Check if window is hidden
bool Window::isHidden() {
	if (SDL_GetWindowFlags(this->window) & SDL_WINDOW_HIDDEN)
		return true;
	return false;
}

// [void] Make OpenGL context current
void Window::useContext() {
	SDL_GL_MakeCurrent(this->window, this->context);
}

// [bool] Check if event involved this window
bool Window::handleEvent(SDL_Event event) {
	if (event.window.windowID && event.window.windowID == SDL_GetWindowID(this->window)) {
		if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				glViewport(0, 0, event.window.data1, event.window.data2);
			}
		}

		return true;
	}
}

// [SDL_Window] Get window
SDL_Window* Window::getWindow() {
	return this->window;
}

// [SDL_GLContext] Get OpenGL context
SDL_GLContext* Window::getContext() {
	return &this->context;
}
