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

// [void] Validate email and url (name is already validated by main regex)
void Person::validate() {
	// Ensure that email is valid if not empty
	if (!this->_email.empty()) {
		std::regex rgx(R"(^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.)"
			R"([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$)");

		if (!std::regex_match(this->_email, rgx))
			throw "MUtilities::Person::invalidEmail";
	}

	// Ensure that url is valid if not empty
	if (!this->_url.empty()) {
		std::regex rgx(R"(^([a-z0-9+.-]+):(?://(?:((?:[a-z0-9-._~!$&'()*+,;=:]|%[0-9A-F]{2})*)@)?)"
			R"(((?:[a-z0-9-._~!$&'()*+,;=]|%[0-9A-F]{2})*)(?::(\d*))?(/(?:[a-z0-9-._~!$&'()*+,;=:@/]|)"
			R"(%[0-9A-F]{2})*)?|(/?(?:[a-z0-9-._~!$&'()*+,;=:@]|%[0-9A-F]{2})+(?:[a-z0-9-._~!$&'()*+,;=:@/]|)"
			R"(%[0-9A-F]{2})*)?)(?:\?((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?)"
			R"((?:#((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?$)");

		if (!std::regex_match(this->_url, rgx)) {
			throw "MUtilities::Person::invalidUrl";
		}
	}
}
