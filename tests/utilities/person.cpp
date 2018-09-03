#include <catch2/catch.hpp>

#include <json/json.h>
#include <utilities/utility.hpp>

#include <utilities/person.hpp>

using namespace MUtilities;

TEST_CASE("person objects can be created from a JSON object", "[person]") {
	SECTION("the name field is required") {
		Person person(Utility::stojson("{\"name\": \"John Doe\"}"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email().empty());
		REQUIRE(person.url().empty());
		REQUIRE_THROWS(Person(Utility::stojson("{}")));
		REQUIRE_THROWS(Person(Utility::stojson("{\"email\": \"j@doe.com\"}")));
		REQUIRE_THROWS(Person(Utility::stojson("{\"url\": \"http://doe.com\"}")));
		REQUIRE_THROWS(Person(Utility::stojson("{\"email\": \"j@doe.com\", \"url\": \"http://doe.com\"}")));
		// Ensure that name is properly validated
		REQUIRE_NOTHROW(Person(Utility::stojson("{\"name\": \"Dr. John A. Doe\"}")));
		REQUIRE_THROWS(Person(Utility::stojson("{\"name\": \"J0hn Do3\"}")));
	}

	SECTION("an email can also be optionally provided") {
		Person person(Utility::stojson("{\"name\": \"John Doe\", \"email\": \"j@doe.com\"}"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email() == "j@doe.com");
		REQUIRE(person.url().empty());
		// Ensure that email is properly validated
		REQUIRE_THROWS(Person(Utility::stojson("{\"name\": \"John Doe\", \"email\": \"jdoe.com\"}")));
		REQUIRE_THROWS(Person(Utility::stojson("{\"name\": \"John Doe\", \"email\": \"j@@doe.com\"}")));
		REQUIRE_THROWS(Person(Utility::stojson("{\"name\": \"John Doe\", \"email\": \"j@https://doe.com\"}")));
	}

	SECTION("a url can also be optionally provided") {
		Person person(Utility::stojson("{\"name\": \"John Doe\", \"url\": \"http://doe.com\"}"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email().empty());
		REQUIRE(person.url() == "http://doe.com");
		// Ensure that URL is properly validated
		REQUIRE_THROWS(Person(Utility::stojson("{\"name\": \"John Doe\", \"url\": \"doe.com\"}")));
		REQUIRE_THROWS(Person(Utility::stojson("{\"name\": \"John Doe\", \"url\": \"www.doe.com\"}")));
		REQUIRE_NOTHROW(Person(Utility::stojson(R"({"name": "John Doe", "url": "http://hello.world.doe.com"})")));
	}

	SECTION("name, email, and url can all be provided at the same time") {
		Person p1(Utility::stojson(R"({"name": "John Doe", "email": "j@doe.com", "url": "http://doe.com"})"));
		REQUIRE(p1.name() == "John Doe");
		REQUIRE(p1.email() == "j@doe.com");
		REQUIRE(p1.url() == "http://doe.com");
	}
}

TEST_CASE("person objects can be created from strings", "[person]") {
	SECTION("the name field is required") {
		Person person(std::string("John Doe"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email().empty());
		REQUIRE(person.url().empty());
		REQUIRE_THROWS(Person(std::string("")));
		REQUIRE_THROWS(Person(std::string("<j@doe.com>")));
		REQUIRE_THROWS(Person(std::string("(http://doe.com)")));
		REQUIRE_THROWS(Person(std::string("<j@doe.com> (http://doe.com)")));
	}

	SECTION("an email can also be optionally provided") {
		Person person(std::string("John Doe <j@doe.com>"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email() == "j@doe.com");
		REQUIRE(person.url().empty());
	}

	SECTION("a url can also be optionally provided") {
		Person person(std::string("John Doe (http://doe.com)"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email().empty());
		REQUIRE(person.url() == "http://doe.com");
	}

	SECTION("name, email, and url can all be provided at the same time") {
		Person p1(std::string("John Doe <j@doe.com> (http://doe.com)"));
		REQUIRE(p1.name() == "John Doe");
		REQUIRE(p1.email() == "j@doe.com");
		REQUIRE(p1.url() == "http://doe.com");
	}
}

TEST_CASE("person objects can be compared", "[person]") {
	SECTION("with other person objects") {
		REQUIRE(Person(std::string("John Doe <j@doe.com>")) == Person(std::string("John Doe <j@doe.com>")));
		REQUIRE_FALSE(Person(std::string("John Doe")) == Person(std::string("John Doe <j@doe.com>")));
		REQUIRE(Person(std::string("John Doe <j@doe.com>")) != Person(std::string("John Doe (https://doe.com)")));
		REQUIRE_FALSE(Person(std::string("John Doe (https://doe.com)")) !=
			Person(std::string("John Doe (https://doe.com)")));
	}

	SECTION("with string representations") {
		REQUIRE(Person(std::string("John Doe <j@doe.com>")) == std::string("John Doe <j@doe.com>"));
		REQUIRE_FALSE(Person(std::string("John Doe")) == std::string("John Doe <j@doe.com>"));
		REQUIRE(Person(std::string("John Doe <j@doe.com>")) != std::string("John Doe (https://doe.com)"));
		REQUIRE_FALSE(Person(std::string("John Doe (https://doe.com)")) !=
			std::string("John Doe (https://doe.com)"));
	}

	SECTION("with JSON representations") {
		REQUIRE(Person(std::string("John Doe <j@doe.com>")) ==
			Utility::stojson(R"({"name": "John Doe", "email": "j@doe.com"})"));
		REQUIRE_FALSE(Person(std::string("John Doe")) ==
			Utility::stojson(R"({"name": "John Doe", "email": "j@doe.com"})"));
		REQUIRE(Person(std::string("John Doe <j@doe.com>")) !=
			Utility::stojson(R"({"name": "John Doe", "url": "https://doe.com"})"));
		REQUIRE_FALSE(Person(std::string("John Doe (https://doe.com)")) !=
			Utility::stojson(R"({"name": "John Doe", "url": "https://doe.com"})"));
	}
}
