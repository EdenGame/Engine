#include <catch2/catch.hpp>

#include <utilities/validate.hpp>

using namespace MUtilities;

TEST_CASE("emails must follow specification", "[validate]") {
	REQUIRE_FALSE(Validate::email("jdoe.com"));
	REQUIRE_FALSE(Validate::email("j@@doe.com"));
	REQUIRE_FALSE(Validate::email("j@https://doe.com"));
	REQUIRE(Validate::email("j@doe.com"));
}

TEST_CASE("urls must follow the uri scheme specification", "[validate]") {
	REQUIRE_FALSE(Validate::url("doe.com"));
	REQUIRE_FALSE(Validate::url("www.doe.com"));
	REQUIRE(Validate::url("https://doe.com"));
	REQUIRE(Validate::url("http://hello.world.doe.com"));
}

TEST_CASE("package names must be url-safe and readable", "[validate]") {
	SECTION("names cannot begin with an underscore, a dash, or a dot (_, -, .)") {
		REQUIRE_FALSE(Validate::packageName("_name"));
		REQUIRE_FALSE(Validate::packageName("-name"));
		REQUIRE_FALSE(Validate::packageName(".name"));
		REQUIRE(Validate::packageName("name-name_name.name"));
	}

	SECTION("names may not contain uppercase characters") {
		REQUIRE_FALSE(Validate::packageName("NaMe"));
		REQUIRE(Validate::packageName("name"));
	}

	SECTION("names may include numbers (0-9), underscores, dashes, and dots") {
		REQUIRE(Validate::packageName("package-2_more.io"));
	}
}
