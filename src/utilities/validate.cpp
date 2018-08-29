#include "validate.hpp"

namespace MUtilities {
	namespace Validate {
		// [bool] Validate Email Address
		bool email(std::string email) {
			std::regex rgx(R"(^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.)"
				R"([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$)");

			if (!std::regex_match(email, rgx))
				return false;

			return true;
		}

		// [bool] Validate URL (with URI scheme)
		bool url(std::string url) {
			std::regex rgx(R"(^([a-z0-9+.-]+):(?://(?:((?:[a-z0-9-._~!$&'()*+,;=:]|%[0-9A-F]{2})*)@)?)"
				R"(((?:[a-z0-9-._~!$&'()*+,;=]|%[0-9A-F]{2})*)(?::(\d*))?(/(?:[a-z0-9-._~!$&'()*+,;=:@/]|)"
				R"(%[0-9A-F]{2})*)?|(/?(?:[a-z0-9-._~!$&'()*+,;=:@]|%[0-9A-F]{2})+(?:[a-z0-9-._~!$&'()*+,;=:@/]|)"
				R"(%[0-9A-F]{2})*)?)(?:\?((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?)"
				R"((?:#((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?$)");

			if (!std::regex_match(url, rgx))
				return false;

			return true;
		}
	}
}
