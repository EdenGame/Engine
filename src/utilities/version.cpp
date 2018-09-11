#include "version.hpp"

using namespace MUtilities;

// Define within MUtilities namespace.
namespace MUtilities {
	// [Version] Convert string to a version object
	void stringToVersion(Version &vobj, std::string version) {
		int major = -1;
		int minor = -1;
		int patch = -1;
		Version::Prerelease prerelease;
		std::string meta;

		std::string last = "";
		// States: 0 - major, 1 - minor, 2 - patch, 3 - prerelease type, 4 - prerelease version, 5 - meta
		unsigned int state = 0;

		// Loop through characters of version string
		for (std::string::size_type i = 0; i < version.size(); ++i) {
			char c = version[i];

			// Ensure that there are no leading zeros
			if (last == "0" && (c != '.' && c != '-' && c != '+')) {
				throw "Error::MUtilities::stringToVersion::leadingZerosDisallowed";
			}

			// Catch exceptions raised by stoi when the string does not represent a number
			try {
				// Check for extra `.`s
				if ((state == 2) && c == '.')
					throw 1;

				// Handle major version
				if (state == 0 && c == '.') {
					major = std::stoi(last);
					last = "";
					state = 1;
				// Handle minor version
				} else if (state == 1 && c == '.') {
					minor = std::stoi(last);
					last = "";
					state = 2;
				// Handle patch version
				} else if (state == 2 && (i == (version.size() - 1) || c == '-' || c == '+')) {
					if (i == (version.size() - 1)) last += c;
					patch = std::stoi(last);
					last = "";
					if (c == '-')
						state = 3;
					else if (c == '+')
						state = 5;
				// Handle prerelease type
				} else if (state == 3 && (i == (version.size() - 1) || c == '.' || c == '+')) {
					if (i == (version.size() - 1)) last += c;
					prerelease.type = last;
					last = "";
					if (c == '.')
						state = 4;
					else if (c == '+')
						state = 5;
				// Handle prerelease version
				} else if (state == 4 && (i == (version.size() - 1) || c == '+')) {
					if (i == (version.size() - 1)) last += c;
					prerelease.version = std::stoi(last);
					last = "";
					state = 5;
				// Handle build metadata
				} else if (state == 5 && i == (version.size() - 1)) {
					meta = last + c;
					break;
				} else { // else, append to last
					last += c;
				}
			} catch (...) {
				throw "Error::MUtilities::stringToVersion::invalidVersionNumber";
			}
		}

		// Ensure major, minor, and patch always exist
		if (major == -1 || minor == -1 || patch == -1) {
			throw "Error::MUtilities::stringToVersion::invalidVersionStructure";
		}

		// Ensure prerelease version is not zero
		if (prerelease.version == 0) {
			throw "Error::MUtilities::stringToVersion::prereleaseVersionIsZero";
		}

		// Ensure build metadata comprises of only alphanumeric characters and hyphens
		if (!meta.empty()) {
			for (char &c: meta) {
				if (!std::isalnum(c) && c != '-' && c != '.') {
					throw "Error::MUTilities::stringToVersion::invalidBuildMeta";
				}
			}
		}

		vobj._major = major;
		vobj._minor = minor;
		vobj._patch = patch;
		vobj._prerelease = prerelease;
		vobj._meta = meta;
		vobj._prerelease.setTypeWeight();
	}
}

/* Version Class */

// [constructor] Clean with defaults
Version::Version(): _major(0), _minor(1), _patch(0) {}
// [constructor] From string
Version::Version(std::string version) {
	this->setFromString(*this, version);
}

// Define within MUtilities namespace.
namespace MUtilities {
	// [std::ostream&] Ostream operator overload using object
	std::ostream& operator << (std::ostream &strm, Version &a) {
		std::string pre = "";
		// Get prerelease information
		if (!a._prerelease.type.empty()) {
			pre = "-" + a._prerelease.type;
			if (a._prerelease.version > 0) {
				pre += "." + std::to_string(a._prerelease.version);
			}
		}

		std::string meta = "";
		// Get metadata information
		if (!a._meta.empty()) {
			meta = "+" + a._meta;
		}

		return strm << a._major << "." << a._minor << "." << a._patch << pre << meta;
	}
}

// [void] Assignment operator overload using string
void Version::operator = (const std::string &n) {
	this->setFromString(*this, n);
}

// [bool] Equality operator overload using object
bool Version::operator == (const Version &b) const {
	bool pre = this->_prerelease == b._prerelease;
	//std::cout << "Prerelease Equality: " << pre << ", " << this->_prerelease.type << ", " <<
//		this->_prerelease.version << "; " << b._prerelease.type << ", " << b._prerelease.version << std::endl;
	return (this->_major == b._major) && (this->_minor == b._minor) && (this->_patch == b._patch) && pre;
}

// [bool] Equality operator overload using string
bool Version::operator == (const std::string &b) const {
	Version obj;
	stringToVersion(obj, b);
	return *this == obj;
}

// [bool] Inequality operator overload using object
bool Version::operator != (const Version &b) const {
	return !(*this == b);
}

// [bool] Inequality operator overload using string
bool Version::operator != (const std::string &b) const {
	Version obj;
	stringToVersion(obj, b);
	return *this != obj;
}

// [bool] Less than operator overload using object
bool Version::operator < (const Version &b) const {
	if (this->_major < b._major)
		return true;
	else if (this->_major == b._major && this->_minor < b._minor)
		return true;
	else if (this->_major == b._major && this->_minor == b._minor && this->_patch < b._patch)
		return true;
	else if (this->_major == b._major && this->_minor == b._minor && this->_patch == b._patch &&
			this->_prerelease < b._prerelease)
		return true;

	return false;
}

// [bool] Less than operator overload using string
bool Version::operator < (const std::string &b) const {
	Version obj;
	stringToVersion(obj, b);
	return *this < obj;
}

// [bool] Greater than operator overload using object
bool Version::operator > (const Version &b) const {
	return !(*this < b) && *this != b;
}

// [bool] Greater than operator overload using string
bool Version::operator > (const std::string &b) const {
	Version obj;
	stringToVersion(obj, b);
	return *this > obj;
}

// [bool] Less than or equal to operator overload using object
bool Version::operator <= (const Version &b) const {
	return *this < b || *this == b;
}

// [bool] Less than or equal to operator overload using string
bool Version::operator <= (const std::string &b) const {
	Version obj;
	stringToVersion(obj, b);
	return *this <= obj;
}

// [bool] Greater than or equal to operator overload using object
bool Version::operator >= (const Version &b) const {
	return !(*this <= b) || *this == b;
}

// [bool] Greater than or equal to operator overload using string
bool Version::operator >= (const std::string &b) const {
	Version obj;
	stringToVersion(obj, b);
	return *this >= obj;
}

// [unsigned int] Get major version
unsigned int Version::major() {
	return this->_major;
}
// [unsigned int] Get minor version
unsigned int Version::minor() {
	return this->_minor;
}
// [unsigned int] Get patch version
unsigned int Version::patch() {
	return this->_patch;
}
// [Prerelease] Get prerelease data
std::pair<std::string, int> Version::prerelease() {
	return std::pair(this->_prerelease.type, this->_prerelease.version);
}
// [string] Get build metadata
std::string Version::meta() {
	return this->_meta;
}

// [void] Set object properties based on string
void Version::setFromString(Version &vobj, std::string version) {
	stringToVersion(*this, version);
}

/* Prerelease Class */

// [constructor] Clean
Version::Prerelease::Prerelease() {}
// [constructor] With type and version
Version::Prerelease::Prerelease(std::string t, unsigned int v): type(t), version(v) {
	this->setTypeWeight();
}

// [unsigned int] Get type importance/weight
void Version::Prerelease::setTypeWeight() {
	if (!this->type.empty()) {
		if (this->type == "alpha")
			this->typeWeight = 0;
		else if (this->type == "beta")
			this->typeWeight = 1;
		else if (this->type == "rc")
			this->typeWeight = 2;
		else {
			std::cerr << "Version::Prerelease::setTypeWeight::invalidType" << std::endl;
			std::exit(1);
		}
	}
}

// [bool] Equality operator overload using object
bool Version::Prerelease::operator == (const Prerelease &b) const {
	return (this->type == b.type) && (this->version == b.version);
}

// [bool] Less than operator overload using object
bool Version::Prerelease::operator < (const Prerelease &b) const {
	// if neither have prerelease data, return false
	if (this->type.empty() && this->version == -1 && b.type.empty() && b.version == -1)
		return false;

	// if there is no prerelease data, return true
	if (b.type.empty() && b.version == -1)
		return true;

	// if this type weight is less than the other, return true
	if (this->typeWeight < b.typeWeight)
		return true;

	// if this weight type is equal to the other, compare prerelease versions
	if (this->typeWeight == b.typeWeight) {
		// if there is no version information or this version is less than the other, return true
		if ((this->version == -1 && b.version > 0) || this->version < b.version)
			return true;
	}

	return false;
}
