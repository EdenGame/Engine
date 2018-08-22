#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <utility>
#include <algorithm>

#undef major // Undefine major for use in class
#undef minor // Undefine minor for use in class

namespace MUtilities {

enum VersionType {MAJOR, MINOR, PATCH, PRETYPE, PREVERSION};

class Version {
	public:
		Version();
		Version(std::string version);

		friend std::ostream& operator << (std::ostream &strm, Version &a); // Ostream
		void operator = (const std::string &n); // Assignment with string
		bool operator == (const Version &b) const; // Equality
		bool operator == (const std::string &b) const; // Equality with string
		bool operator != (const Version &b) const; // Inequality
		bool operator != (const std::string &b) const; // Inequality with string
		bool operator < (const Version &b) const; // Less than
		bool operator < (const std::string &b) const; // Less than with string
		bool operator > (const Version &b) const; // Greater than
		bool operator > (const std::string &b) const; // Greater than with string
		bool operator <= (const Version &b) const; // Less than or equal to
		bool operator <= (const std::string &b) const; // Less than or equal to with string
		bool operator >= (const Version &b) const; // Greater than or equal to
		bool operator >= (const std::string &b) const; // Greater than or equal to  with string

		unsigned int major();
		unsigned int minor();
		unsigned int patch();
		std::pair<std::string, int> prerelease();
		std::string meta();

		friend void stringToVersion(Version &vobj, std::string version);
	private:
		class Prerelease {
			public:
				Prerelease();
				Prerelease(std::string t, unsigned int v);

				std::string type;
				int version = -1;
				int typeWeight = -1;

				bool operator == (const Prerelease &b) const;
				bool operator < (const Prerelease &b) const;

				void setTypeWeight();
		};

		unsigned int _major;
		unsigned int _minor;
		unsigned int _patch;
		Prerelease _prerelease;
		std::string _meta;

		void setFromString(Version &vobj, std::string version);
};

}
