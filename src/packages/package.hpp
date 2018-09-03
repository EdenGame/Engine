#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <json/json.h>

#include "../utilities/person.hpp"
#include "../utilities/range.hpp"
#include "../utilities/version.hpp"
#include "../utilities/utility.hpp"

namespace MPackages {

class Package {
	public:
		Package(std::string path, Json::Value pkg);
		~Package();

		// Getters
		std::string path() const;
		std::string name() const;
		MUtilities::Version version() const;
		std::string title() const;
		std::string description() const;
		std::vector<std::string> keywords() const;
		std::string homepage() const;
		std::pair<std::string, std::string> bugs() const;
		std::string license() const;
		MUtilities::Person author() const;
		std::vector<MUtilities::Person> contributors() const;
		std::string repository() const;
		std::map<std::string, MUtilities::Range> dependencies() const;
		std::map<std::string, MUtilities::Range> optionalDependencies() const;
		bool isPrivate() const;

		virtual bool run() = 0;
	private:
		// [nested class] Bugs
		class Bugs {
			public:
				std::string email;
				std::string url;
		};

		std::string _path;
		std::string _name; // Package name must be validated
		MUtilities::Version _version;
		std::string _title;
		std::string _description;
		std::vector<std::string> _keywords;
		std::string _homepage; // URL must be validated
		Bugs _bugs; // Email and URL must be validated
		std::string _license;
		MUtilities::Person *_author = nullptr;
		std::vector<MUtilities::Person> _contributors;
		std::string _repository; // URL must be validated and end with .git
		std::map<std::string, MUtilities::Range> _dependencies; // Dependency names must be validated
		std::map<std::string, MUtilities::Range> _optionalDependencies; // Dependency names must be validated
		bool _private;

		void init(Json::Value pkg);
};

}
