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

		// Operator overlodas
		friend std::ostream& operator << (std::ostream &strm, Person &a); // Ostream
		bool operator == (const Person &b) const; // Equality
		bool operator == (const std::string &b) const; // Equality with string
		bool operator == (const Json::Value &b) const; // Equality with JSON object
		bool operator != (const Person &b) const; // Inequality
		bool operator != (const std::string &b) const; // Inequality with string
		bool operator != (const Json::Value &b) const; // Inequality with JSON object
	private:
		std::string _name = "";
		std::string _email = "";
		std::string _url = "";

		void validate();
};

}
