#include "range.hpp"

using namespace MUtilities;

/* Version Range Class */

// [constructor] Parse range into comparator sets
Range::Range(std::string range) {
	std::string last = "";
	// Loop through characters of range string
	for (std::string::size_type i = 0; i < range.size(); ++i) {
		char c = range[i];

		// if the last character is the beginning of a logical or operator, require next character to finish
		if (last == "|" && c == '|') {
			last = "";
		} else if (last == "|" && c != '|') { // else if | and char is not |, error
			throw "MUtilities::Range::invalidRange";
		} else { // else, treat character normally
			// Ensure that if last is empty the character is not whitespace
			if (!last.empty() || !std::isspace(c)) {
				// Check if character is the beginning of a logical or operator
				if (c == '|' || (i == (range.size() - 1))) {
					if (i == (range.size() - 1)) last += c;
					// if last is not empty, create a comparator set and add to list
					if (!last.empty()) {
						this->sets.push_back(Range::set(last));
					}
					last = "|"; // Reset last to a half logical or operator
				} else { // else, append character to last
					last += c;
				}
			}
		}
	}
}

// [bool] Loop through comparator sets in attempt to find one that is satisfied by the version
bool Range::satisfiedBy(Version version) {
	for (auto const &cs: this->sets) {
		if (cs.satisfiedBy(version))
			return true;
	}
}
// [bool] Convert version string to a version object and check if it satisfies
bool Range::satisfiedBy(std::string version) {
	return this->satisfiedBy(Version(version));
}

// [Version] Find the highest version in the vector of objects that satisfies the range
Version Range::maxSatisfiedBy(std::vector<Version> versions) {
	int highest = -1;
	for (int i = 0; i != versions.size(); i++) {
		if ((highest == -1 || versions[i] > versions[highest]) && this->satisfiedBy(versions[i]))
			highest = i;
	}

	return versions[highest];
}
// [Version] Convert version strings to version objects and find the highest that satisfies the range
Version Range::maxSatisfiedBy(std::vector<std::string> versions) {
	std::vector<Version> passVec;
	// Convert strings to version objects
	for (auto const &s: versions) {
		passVec.push_back(Version(s));
	}
	return this->maxSatisfiedBy(passVec);
}

// [bool] Equality operator overload using object
bool Range::operator == (const Range &b) const {
	for (std::vector<set>::size_type i = 0; i != this->sets.size(); i++) {
		// Ensure b has the same number of sets as this
		if (this->sets.size() != b.sets.size()) {
			return false;
		}

		set s = this->sets[i];
		for (std::vector<comparator>::size_type j = 0; j != s.comparators.size(); j++) {
			// Ensure b has the same number of comparators as this
			if (s.comparators.size() != b.sets[i].comparators.size()) {
				return false;
			}

			comparator c = s.comparators[j];
			comparator bc = b.sets[i].comparators[j];

			if (c.oper != bc.oper || c.version != bc.version) {
				return false;
			}
		}
	}

	return true;
}
// [bool] Equality operator overload using string
bool Range::operator == (const std::string &b) const {
	Range obj = Range(b);
	return *this == obj;
}
// [bool] Inequality operator overload using object
bool Range::operator != (const Range &b) const {
	return !(*this == b);
}
// [bool] Inequality operator overload using string
bool Range::operator != (const std::string &b) const {
	Range obj = Range(b);
	return *this != obj;
}

/* Version Comparator Set Class */

// [constructor] Parse comparator set into individual comparators
Range::set::set(std::string s) {
	std::string last = "";
	// Loop through characters of comparator set string
	for (std::string::size_type i = 0; i < s.size(); ++i) {
		char c = s[i];

		// if c is a space and last is not empty or last index, create comparator object and add to list
		if ((c == ' ' && !last.empty()) || (i == (s.size() - 1))) {
			if (i == (s.size() - 1)) last += c;
			this->comparators.push_back(Range::comparator(last));
			last = "";
		// else if last is not empty or character is not whitespace, add to last
		} else if ((!last.empty() || !std::isspace(c))) {
			last += c;
		} else {
			throw "MUtilities::Range::set::invalidComparatorSet";
		}
	}
}

// [bool] Loop through comparators and ensure that all are satisfied by the version
bool Range::set::satisfiedBy(const Version version) const {
	for (auto const &c: this->comparators) {
		if (!c.satisfiedBy(version))
			return false;
	}

	return true;
}

/* Version Comparator Class */

// [constructor] Parse comparator into an operator and a version
Range::comparator::comparator(std::string comp) {
	std::regex rgx(R"(^\s*(>|<|<=|>=|=|)([\w\.\-\+]+)\s*$)");
	std::smatch match;

	if (std::regex_search(comp, match, rgx)) {
		if (match[1] == "") {
			this->oper = "=";
		} else {
			this->oper = match[1];
		}
		this->version = Version(match[2]);
	} else {
		throw "MUtilities::Range::comparator::invalidComparatorString\n\t\"" + comp + "\"";
	}
}

// [bool] Check if comparator is satisfied by the version
bool Range::comparator::satisfiedBy(Version v) const {
	if (this->oper == "<")
		return v < this->version;
	else if (this->oper == "<=")
		return v <= this->version;
	else if (this->oper == ">")
		return v > this->version;
	else if (this->oper == ">=")
		return v >= this->version;
	else if (this->oper == "=")
		return v == this->version;
}
