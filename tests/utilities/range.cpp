#include <catch2/catch.hpp>

#include <utilities/range.hpp>
#include <utilities/version.hpp>

using namespace MUtilities;

TEST_CASE("ranges can be created with a single comparator", "[range]") {
	SECTION("less than operator") {
		Range range("<1.2.3");
		REQUIRE(range.satisfiedBy("0.9.6"));
		REQUIRE_FALSE(range.satisfiedBy("3.2.8"));
		REQUIRE_FALSE(range.satisfiedBy("1.2.3"));

		Range r2("<2.8.4-alpha.3");
		REQUIRE(r2.satisfiedBy("2.8.4-alpha.2"));
		REQUIRE_FALSE(r2.satisfiedBy("2.8.4-beta.1"));

		std::vector<std::string> vec;
		vec.push_back("0.5.2");
		vec.push_back("4.9.6");
		vec.push_back("1.0.5");
		Version v = range.maxSatisfiedBy(vec);
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 0);
		REQUIRE(v.patch() == 5);
	}

	SECTION("less than or equal to operator") {
		Range range("<=1.2.3");
		REQUIRE(range.satisfiedBy("0.9.6"));
		REQUIRE(range.satisfiedBy("1.2.3"));
		REQUIRE_FALSE(range.satisfiedBy("3.2.8"));

		std::vector<std::string> vec;
		vec.push_back("0.5.2");
		vec.push_back("4.9.6");
		vec.push_back("1.0.5");
		vec.push_back("1.2.3");
		Version v = range.maxSatisfiedBy(vec);
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 2);
		REQUIRE(v.patch() == 3);
	}

	SECTION("greater than operator") {
		Range range(">1.2.3");
		REQUIRE(range.satisfiedBy("3.9.4"));
		REQUIRE_FALSE(range.satisfiedBy("0.9.6"));
		REQUIRE_FALSE(range.satisfiedBy("1.2.3"));

		std::vector<std::string> vec;
		vec.push_back("4.9.6");
		vec.push_back("1.0.5");
		vec.push_back("9.8.6");
		Version v = range.maxSatisfiedBy(vec);
		REQUIRE(v.major() == 9);
		REQUIRE(v.minor() == 8);
		REQUIRE(v.patch() == 6);
	}

	SECTION("greater than or equal to operator") {
		Range range(">=1.2.3");
		REQUIRE(range.satisfiedBy("3.9.4"));
		REQUIRE(range.satisfiedBy("1.2.3"));
		REQUIRE_FALSE(range.satisfiedBy("0.9.6"));

		std::vector<std::string> vec;
		vec.push_back("4.9.6");
		vec.push_back("1.0.5");
		vec.push_back("9.8.6");
		Version v = range.maxSatisfiedBy(vec);
		REQUIRE(v.major() == 9);
		REQUIRE(v.minor() == 8);
		REQUIRE(v.patch() == 6);
	}

	SECTION("equality operator") {
		Range range("=1.2.3");
		REQUIRE(range.satisfiedBy("1.2.3"));
		REQUIRE_FALSE(range.satisfiedBy("0.9.6"));
		REQUIRE_FALSE(range.satisfiedBy("3.2.1"));

		std::vector<std::string> vec;
		vec.push_back("4.9.6");
		vec.push_back("1.0.5");
		vec.push_back("9.8.6");
		Version v = range.maxSatisfiedBy(vec);
		REQUIRE(v.major() == 0);
		REQUIRE(v.minor() == 0);
		REQUIRE(v.patch() == 0);

		SECTION("if no operator is provided, equality is assumed") {
			Range r2("1.2.3");
			REQUIRE(r2.satisfiedBy("1.2.3"));
			REQUIRE_FALSE(r2.satisfiedBy("0.9.6"));
			REQUIRE_FALSE(r2.satisfiedBy("3.2.1"));
		}
	}
}

TEST_CASE("ranges can be created with multiple comparators", "[range]") {
	Range r1(">4.2.8 <9.6.2");
	REQUIRE(r1.satisfiedBy("5.8.2"));
	REQUIRE_FALSE(r1.satisfiedBy("4.2.8"));
	REQUIRE_FALSE(r1.satisfiedBy("10.2.4"));

	Range r2("<=2.9.5 >0.4.2");
	REQUIRE(r2.satisfiedBy("1.67.2"));
	REQUIRE(r2.satisfiedBy("2.6.3-rc.5"));
	REQUIRE_FALSE(r2.satisfiedBy("3.74.2"));
}

TEST_CASE("ranges can be created with logical ORs", "[range]") {
	Range r1("1.4.6 || >=1.9.2 <3.0.0");
	REQUIRE(r1.satisfiedBy("1.4.6"));
	REQUIRE_FALSE(r1.satisfiedBy("1.6.0"));
	REQUIRE(r1.satisfiedBy("2.0.0"));
	REQUIRE_FALSE(r1.satisfiedBy("3.0.0"));
	REQUIRE_FALSE(r1.satisfiedBy("4.1.0"));

	Range r2("1.5.2 || >9.4.2 <12.4.2 || 9.2.5 || >=25.0.0");
	REQUIRE(r2.satisfiedBy("1.5.2"));
	REQUIRE(r2.satisfiedBy("9.2.5"));
	REQUIRE(r2.satisfiedBy("25.0.0"));
	REQUIRE(r2.satisfiedBy("10.2.4"));
	REQUIRE_FALSE(r2.satisfiedBy("15.67.2"));
	REQUIRE_FALSE(r2.satisfiedBy("5.3.8"));
}

TEST_CASE("range objects can be compared", "[range]") {
	SECTION("with other range objects") {
		REQUIRE(Range(">4.2.8 <9.6.2") == Range(">4.2.8 <9.6.2"));
		REQUIRE_FALSE(Range(">4.2.8 <9.6.2") == Range(">4.2.8"));
		REQUIRE_FALSE(Range("1.4.6 || >=1.9.2 <3.0.0") == Range(">1.4.6 || <2.5.4 >2.0.0 || 0.6.3"));
		REQUIRE(Range(">4.2.8") != Range("<1.4.5"));
		REQUIRE_FALSE(Range(">6.8.3 <9.1.3") != Range(">6.8.3 <9.1.3"));
	}

	SECTION("with strings") {
		REQUIRE(Range(">4.2.8 <9.6.2") == ">4.2.8 <9.6.2");
		REQUIRE_FALSE(Range(">4.2.8 <9.6.2") == ">4.2.8");
		REQUIRE_FALSE(Range("1.4.6 || >=1.9.2 <3.0.0") == ">1.4.6 || <2.5.4 >2.0.0 || 0.6.3");
		REQUIRE(Range(">4.2.8") != "<1.4.5");
		REQUIRE_FALSE(Range(">6.8.3 <9.1.3") != ">6.8.3 <9.1.3");
	}
}
