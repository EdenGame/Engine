#include "window.hpp"

using namespace MClient;

/* Window Class */

windowPos::windowPos() {}
windowPos::windowPos(int x, int y): x(x), y(y) {}
windowSize::windowSize() {}
windowSize::windowSize(int w, int h): w(w), h(h) {}

Window::Window() { this->init(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 576); }
// With name
Window::Window(std::string name): name(name) {
	this->init(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 576);
}
// With name and position struct
Window::Window(std::string name, windowPos winPos): name(name) { this->init(winPos.x, winPos.y, 1024, 576); }
// With name and position floats
Window::Window(std::string name, float x, float y): name(name) {
	this->init(x, y, 1024, 576);
}
// With name, position struct, and size struct
Window::Window(std::string name, windowPos winPos, windowSize winSize): name(name) {
	this->init(winPos.x, winPos.y, winSize.w, winSize.h);
}
// With name, position and size floats
Window::Window(std::string name, float x, float y, float w, float h): name(name) {
	this->init(x, y, w, h);
}
Window::~Window() {}

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
windowPos Window::getPos() {
	int x = 0;
	int y = 0;
	SDL_GetWindowPosition(this->window, &x, &y);
	return windowPos(x, y);
}
// [void] Set window position with struct
void Window::setPos(windowPos newPos) {
	SDL_SetWindowPosition(this->window, newPos.x, newPos.y);
}
// [void] Set window position with floats
void Window::setPos(float x, float y) {
	SDL_SetWindowPosition(this->window, x, y);
}

// [windowSize] Get window size
windowSize Window::getSize() {
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(this->window, &w, &h);
	return windowSize(w, h);
}
// [void] Set window size with struct
void Window::setSize(windowSize newSize) {
	SDL_SetWindowSize(this->window, newSize.w, newSize.h);
}
// [void] Set window size with floats
void Window::setSize(float w, float h) {
	SDL_SetWindowSize(this->window, w, h);
}

// [void] Hide window
void Window::hide() {
	SDL_HideWindow(this->window);
}
// [void] Show window
void Window::show() {
	SDL_ShowWindow(this->window);
}
// [void] Minimize window
void Window::minimize() {
	SDL_MinimizeWindow(this->window);
}
// [void] Maximize window
void Window::maximize() {
	SDL_MaximizeWindow(this->window);
}
// [void] Restore window
void Window::restore() {
	SDL_RestoreWindow(this->window);
}
// [void] Close window
void Window::close() {
	SDL_DestroyWindow(this->window);
}

// [void] Make OpenGL context current
void Window::makeContextCurrent() {
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

// [void] Initialize window
void Window::init(float x, float y, float w, float h) {
	this->window = SDL_CreateWindow(this->name.c_str(), x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// Create OpenGL Context
	this->context = SDL_GL_CreateContext(this->window);
	SDL_GL_MakeCurrent(this->window, this->context);
}
