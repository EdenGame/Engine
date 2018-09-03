#include "person.hpp"

using namespace MUtilities;

// Define within MUtilities namespace.
namespace MUtilities {
	// [std::ostream&] Ostream operator overload using person object
	std::ostream& operator << (std::ostream &strm, Person &a) {
		std::string email = "";
		std::string url = "";

		if (!a._email.empty())
			email = " <" + a._email + ">";

		if (!a._url.empty())
			url = " (" + a._url + ")";

		return strm << a._name << email << url;
	}
}

/* Person Class */

// [constructor] JSON-based
Person::Person(Json::Value obj) {
	// if name is not null, ensure it is a string and use
	if (!obj["name"].isNull()) {
		if (!obj["name"].isString())
			throw "MUtilities::Person::nameMustBeString";

		if (!std::regex_match(obj["name"].asString(), std::regex(R"(^([a-zA-Z\.\s]+?)$)")))
			throw "MUtilities::Person::invalidName";

		this->_name = obj["name"].asString();
	} else {
		throw "MUtilities::Person::nameCannotBeNull";
	}
	// if email is not null, ensure it is a string and use
	if (!obj["email"].isNull()) {
		if (!obj["email"].isString())
			throw "MUtilities::Person::emailMustBeString";

		this->_email = obj["email"].asString();
	}
	// if url is not null, ensure it is a string and use
	if (!obj["url"].isNull()) {
		if (!obj["url"].isString())
			throw "MUtilities::Person::urlMustBeString";

		this->_url = obj["url"].asString();
	}

	// Validate all
	this->validate();
}

// [constructor] String-based
Person::Person(std::string str) {
	std::regex rgx(R"(^([a-zA-Z\.\s]+?)(?:\s+<(.*?)>)?(?:\s+\((.*?)\))?$)");
	std::smatch match;

	if (!str.empty() && std::regex_search(str, match, rgx)) {
		this->_name = match[1];
		this->_email = match[2];
		this->_url = match[3];
	} else {
		throw "MUtilities::Person::invalidPersonString";
	}

	// Validate all
	this->validate();
}

// [string] Get name
std::string Person::name() const {
	return this->_name;
}
// [string] Get email
std::string Person::email() const {
	return this->_email;
}
// [string] Get URL
std::string Person::url() const {
	return this->_url;
}

// [bool] Equality operator overload using object
bool Person::operator == (const Person &b) const {
	return (this->_name == b._name) && (this->_email == b._email) && (this->_url == b._url);
}
// [bool] Equality operator overload using string
bool Person::operator == (const std::string &b) const {
	Person obj = Person(b);
	return *this == obj;
}
// [bool] Equality operator overload using JSON object
bool Person::operator == (const Json::Value &b) const {
	Person obj = Person(b);
	return *this == obj;
}
// [bool] Inequality operator overload using object
bool Person::operator != (const Person &b) const {
	return !(*this == b);
}
// [bool] Inequality operator overload using string
bool Person::operator != (const std::string &b) const {
	Person obj = Person(b);
	return *this != obj;
}
// [bool] Inequality operator overload using JSON object
bool Person::operator != (const Json::Value &b) const {
	Person obj = Person(b);
	return *this != obj;
}

// [void] Validate email and url (name is already validated by main regex)
void Person::validate() {
	// Ensure that email is valid if not empty
	if (!this->_email.empty()) {
		if (!Validate::email(this->_email))
			throw "MUtilities::Person::invalidEmail";
	}

	// Ensure that url is valid if not empty
	if (!this->_url.empty()) {
		if (!Validate::url(this->_url))
			throw "MUtilities::Person::invalidUrl";
	}
}
