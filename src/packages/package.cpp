#include "package.hpp"

using namespace MPackages;

/* Abstract Package Class */

// [constructor] Construct from path string and Json object
Package::Package(std::string path, Json::Value pkg) {
	this->_path = path;

	// Extract and validate package name (required)
	if (pkg["name"].isString() && MUtilities::Validate::packageName(pkg["name"].asString())) {
		this->_name = pkg["name"].asString();
	} else {
		throw "MPackages::Package::invalidName";
	}

	// Extract version (required)
	if (pkg["version"].isString()) {
		this->_version = MUtilities::Version(pkg["version"].asString());
	} else {
		throw "MPackages::Package::invalidVersion";
	}

	// Extract title
	if (pkg["title"].isString()) {
		this->_title = pkg["title"].asString();
	} else if (!pkg["title"].isNull()) {
		throw "MPackages::Package::invalidTitle";
	}

	// Extract description
	if (pkg["description"].isString()) {
		this->_description = pkg["description"].asString();
	} else if (!pkg["description"].isNull()) {
		throw "MPackages::Package::invalidDescription";
	}

	// Extract keywords
	if (pkg["keywords"].isArray()) {
		for (Json::Value::ArrayIndex i = 0; i != pkg["keywords"].size(); i++) {
			if (pkg["keywords"][i].isString()) {
				this->_keywords.push_back(pkg["keywords"][i].asString());
			} else {
				throw "MPackages::Package::invalidKeyword";
			}
		}
	} else if (!pkg["keywords"].isNull()) {
		throw "MPackages::Package::invalidKeywordList";
	}

	// Extract homepage
	if (pkg["homepage"].isString() && MUtilities::Validate::url(pkg["homepage"].asString())) {
		this->_homepage = pkg["homepage"].asString();
	} else {
		if (!pkg["homepage"].isNull()) {
			throw "MPackages::Package::invalidHomepage";
		}
	}

	// Extract bug reporting information
	if (pkg["bugs"].isObject()) {
		// Extract URL
		if (pkg["bugs"]["url"].isString() && MUtilities::Validate::url(pkg["bugs"]["url"].asString())) {
			this->_bugs.url = pkg["bugs"]["url"].asString();
		} else {
			throw "MPackages::Package::invalidBugReportUrl";
		}

		// Extract email
		if (pkg["bugs"]["email"].isString() && MUtilities::Validate::email(pkg["bugs"]["email"].asString())) {
			this->_bugs.email = pkg["bugs"]["email"].asString();
		} else {
			throw "MPackages::Package::invalidBugReportemail";
		}
	} else if (!pkg["bugs"].isNull()) {
		throw "MPackages::Package::invalidBugsInformation";
	}

	// Extract license (note: validation for this field is not yet complete)
	if (pkg["license"].isString()) {
		this->_license = pkg["license"].asString();
	} else if (!pkg["license"].isNull()) {
		throw "MPackages::Package::invalidLicense";
	}

	// Extract author
	if (pkg["author"].isObject()) {
		this->_author = new MUtilities::Person(pkg["author"]);
	} else if (pkg["author"].isString()) {
		this->_author = new MUtilities::Person(pkg["author"].asString());
	} else if (!pkg["author"].isNull()) {
		throw "MPackages::Package::invalidAuthor";
	}

	// Extract contributors
	if (pkg["contributors"].isArray()) {
		for (Json::Value::ArrayIndex i = 0; i != pkg["contributors"].size(); i++) {
			if (pkg["contributors"][i].isObject()) {
				this->_contributors.push_back(MUtilities::Person(pkg["contributors"][i]));
			} else if (pkg["contributors"][i].isString()) {
				this->_contributors.push_back(MUtilities::Person(pkg["contributors"][i].asString()));
			} else {
				throw "MPackages::Package::invalidContributor";
			}
		}
	} else if (!pkg["contributors"].isNull()) {
		throw "MPackages::Package::invalidContributorList";
	}

	// Extract repository
	if (pkg["repository"].isString() && MUtilities::Validate::url(pkg["repository"].asString()) &&
			MUtilities::Utility::hasSuffix(pkg["repository"].asString(), ".git")) {
		this->_repository = pkg["repository"].asString();
	} else {
		if (!pkg["repository"].isNull()) {
			throw "MPackages::Package::invalidRepository";
		}
	}

	// Extract dependencies
	if (pkg["dependencies"].isObject() && pkg["dependencies"].size() > 0) {
		Json::Value depends = pkg["dependencies"];
		for (auto name: depends.getMemberNames()) {
			Json::Value depend = depends[name];

			if (depend.isString() && MUtilities::Validate::dependName(name)) {
				this->_dependencies.insert(std::pair(name, MUtilities::Range(depend.asString())));
			} else {
				if (!depend.isNull()) {
					throw "MPackages::Package::invalidDependency";
				}
			}
		}
	} else if (!pkg["dependencies"].isNull()) {
		throw "MPackages::Package::invalidDependencyList";
	}

	// Extract optional dependencies
	if (pkg["optionalDependencies"].isObject() && pkg["optionalDependencies"].size() > 0) {
		Json::Value depends = pkg["optionalDependencies"];
		for (auto name: depends.getMemberNames()) {
			Json::Value depend = depends[name];

			if (depend.isString() && MUtilities::Validate::dependName(name)) {
				this->_optionalDependencies.insert(std::pair(name, MUtilities::Range(depend.asString())));
			} else {
				if (!depend.isNull()) {
					throw "MPackages::Package::invalidOptionalDependency";
				}
			}
		}
	} else if (!pkg["optionalDependencies"].isNull()) {
		throw "MPackages::Package::invalidOptionalDependencyList";
	}

	// Extract whether package is private
	if (pkg["private"].isBool()) {
		this->_private = pkg["private"].asBool();
	} else if (!pkg["private"].isNull()) {
		throw "MPackages::Package::invalidPrivateStatus";
	}
}
// [destructor] Delete pointers
Package::~Package() {
	delete this->_author;
}

// [string] Get path
std::string Package::path() const {
	return this->_path;
}
// [string] Get name
std::string Package::name() const {
	return this->_name;
}
// [Version] Get version
MUtilities::Version Package::version() const {
	return this->_version;
}
// [string] Get title
std::string Package::title() const {
	return this->_title;
}
// [string] Get description
std::string Package::description() const {
	return this->_description;
}
// [vector of strings] Get keywords
std::vector<std::string> Package::keywords() const {
	return this->_keywords;
}
// [string] Get homepage
std::string Package::homepage() const {
	return this->_homepage;
}
// [pair of strings] Get bug reporting information
std::pair<std::string, std::string> Package::bugs() const {
	return std::pair(this->_bugs.url, this->_bugs.email);
}
// [string] Get license
std::string Package::license() const {
	return this->_license;
}
// [Person] Get author
MUtilities::Person Package::author() const {
	return *this->_author;
}
// [vector of people] Get contributors
std::vector<MUtilities::Person> Package::contributors() const {
	return this->_contributors;
}
// [string] Get repository
std::string Package::repository() const {
	return this->_repository;
}
// [map of strings to Ranges] Get dependencies
std::map<std::string, MUtilities::Range> Package::dependencies() const {
	return this->_dependencies;
}
// [map of strings to Ranges] Get optional dependencies
std::map<std::string, MUtilities::Range> Package::optionalDependencies() const {
	return this->_optionalDependencies;
}
// [bool] Check if package is private
bool Package::isPrivate() const {
	return this->_private;
}
