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
