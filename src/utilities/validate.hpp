#pragma once

#include <string>
#include <regex>

namespace MUtilities::Validate {
	bool email(std::string email);
	bool url(std::string url);
	bool packageName(std::string name);
	bool dependName(std::string name);
}
