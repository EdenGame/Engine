#pragma once

#include <string>
#include <regex>

namespace MUtilities {
	namespace Validate {
		bool email(std::string email);
		bool url(std::string url);
		bool packageName(std::string name);
	}
}
