#pragma once

#include <json/json.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstddef>
#include <regex>

#include "validate.hpp"

namespace MUtilities {

class Person {
	public:
		Person(Json::Value obj);
		Person(std::string str);

		std::string name() const;
		std::string email() const;
		std::string url() const;

		friend std::ostream& operator << (std::ostream &strm, Person &a); // Ostream operator overload
	private:
		std::string _name = "";
		std::string _email = "";
		std::string _url = "";

		void validate();
};

}
