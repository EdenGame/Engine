#include "utility.hpp"

namespace MUtilities {
	namespace Utility {
		// [Json Value] Parse string to JSON object
		Json::Value stojson(std::string str) {
			Json::CharReaderBuilder builder;
			Json::CharReader *reader = builder.newCharReader();
			std::string errors;

			Json::Value obj;
			reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

			delete reader;
			return obj;
		}

		// [bool] Check if string ends with a suffix
		bool hasSuffix(const std::string &str, const std::string &suffix) {
			return str.size() >= suffix.size() &&
				str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
		}
	}
}
