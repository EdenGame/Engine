#include "utility.hpp"

namespace MUtilities {
	namespace Utility {
		// [Json Value] Pars
		Json::Value stojson(std::string str) {
			Json::CharReaderBuilder builder;
			Json::CharReader *reader = builder.newCharReader();
			std::string errors;

			Json::Value obj;
			reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

			delete reader;
			return obj;
		}
	}
}
