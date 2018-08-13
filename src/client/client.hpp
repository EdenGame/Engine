#pragma once

#include <iostream>

#include "window.hpp"

namespace MClient {

class Client {
	public:
		~Client();
		static Client *getInstance();

		void main();
	private:
		Client();

		Window window;
};

}
