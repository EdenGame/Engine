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
						this->csets.push_back(Range::set(last));
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
	for (auto const &cs: this->csets) {
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
	std::string tempOper;
	char one = comp[0];
	char two = comp[1];

	// There is a single operator character (i.e. <, >, or =)
	if (!std::isalnum(one) && std::isalnum(two)) {
		tempOper = one;
	// There are two operator characters (i.e. <= or >=)
	} else if (!std::isalnum(one) && !std::isalnum(two)) {
		tempOper = std::string() + one + two;
	} else {
		tempOper = '=';
		comp = "=" + comp;
	}

	// Ensure that operator is valid
	if (tempOper == "<" || tempOper == "<=" || tempOper == ">" || tempOper == ">=" || tempOper == "=")
		this->oper = tempOper;
	else
		throw("MUtilities::Range::comparator::invalidComparatorOperator");
	// Set version
	if (tempOper.length() == 1)
		this->version = Version(comp.substr(1, -1));
	else
		this->version = Version(comp.substr(2, -1));
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
