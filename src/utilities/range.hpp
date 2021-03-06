#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <regex>

#include "version.hpp"

namespace MUtilities {
	/* Version Range Class */
	class Range {
		public:
			//Range();
			Range(std::string range);

			bool satisfiedBy(Version version);
			bool satisfiedBy(std::string version);

			Version maxSatisfiedBy(std::vector<Version> versions);
			Version maxSatisfiedBy(std::vector<std::string> versions);

			// Operator overloads
			friend std::ostream& operator << (std::ostream &strm, Range &a); // Ostream
			bool operator == (const Range &b) const; // Equality using object
			bool operator == (const std::string &b) const; // Equality using string
			bool operator != (const Range &b) const; // Inequality using object
			bool operator != (const std::string &b) const; // Inequality using string
		private:
			/* Version Comparator Class */
			class comparator {
				public:
					comparator(std::string c);
					bool satisfiedBy(Version v) const;

					std::string oper;
					Version version;
			};

			/* Version Comparator Set Class */
			class set {
				public:
					set(std::string s);
					bool satisfiedBy(const Version version) const;
					std::vector<comparator> comparators;
			};

			// List of comparator sets as parsed by constructor
			std::vector<set> sets;
	};
}
