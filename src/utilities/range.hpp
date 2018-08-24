#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "version.hpp"

namespace MUtilities {
	/* Version Range Class */
	class Range {
		public:
			Range(std::string range);

			bool satisfiedBy(Version version);
			bool satisfiedBy(std::string version);

			Version maxSatisfiedBy(std::vector<Version> versions);
			Version maxSatisfiedBy(std::vector<std::string> versions);
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
			std::vector<set> csets;
	};
}
