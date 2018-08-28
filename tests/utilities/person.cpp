#include <catch2/catch.hpp>

#include <json/json.h>

#include <utilities/person.hpp>

using namespace MUtilities;

Json::Value parse(std::string str) {
	Json::CharReaderBuilder builder;
	Json::CharReader *reader = builder.newCharReader();
	std::string errors;

	Json::Value obj;
	reader->parse(str.c_str(), str.c_str() + str.size(), &obj, &errors);

	delete reader;
	return obj;
}

TEST_CASE("person objects can be created from a JSON object", "[person]") {
	SECTION("the name field is required") {
		Person person(parse("{\"name\": \"John Doe\"}"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email().empty());
		REQUIRE(person.url().empty());
		REQUIRE_THROWS(Person(parse("{}")));
		REQUIRE_THROWS(Person(parse("{\"email\": \"j@doe.com\"}")));
		REQUIRE_THROWS(Person(parse("{\"url\": \"http://doe.com\"}")));
		REQUIRE_THROWS(Person(parse("{\"email\": \"j@doe.com\", \"url\": \"http://doe.com\"}")));
		// Ensure that name is properly validated
		REQUIRE_NOTHROW(Person(parse("{\"name\": \"Dr. John A. Doe\"}")));
		REQUIRE_THROWS(Person(parse("{\"name\": \"J0hn Do3\"}")));
	}

	SECTION("an email can also be optionally provided") {
		Person person(parse("{\"name\": \"John Doe\", \"email\": \"j@doe.com\"}"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email() == "j@doe.com");
		REQUIRE(person.url().empty());
		// Ensure that email is properly validated
		REQUIRE_THROWS(Person(parse("{\"name\": \"John Doe\", \"email\": \"jdoe.com\"}")));
		REQUIRE_THROWS(Person(parse("{\"name\": \"John Doe\", \"email\": \"j@@doe.com\"}")));
		REQUIRE_THROWS(Person(parse("{\"name\": \"John Doe\", \"email\": \"j@https://doe.com\"}")));
	}

	SECTION("a url can also be optionally provided") {
		Person person(parse("{\"name\": \"John Doe\", \"url\": \"http://doe.com\"}"));
		REQUIRE(person.name() == "John Doe");
		REQUIRE(person.email().empty());
		REQUIRE(person.url() == "http://doe.com");
		// Ensure that URL is properly validated
		REQUIRE_THROWS(Person(parse("{\"name\": \"John Doe\", \"url\": \"doe.com\"}")));
		REQUIRE_THROWS(Person(parse("{\"name\": \"John Doe\", \"url\": \"www.doe.com\"}")));
		REQUIRE_NOTHROW(Person(parse("{\"name\": \"John Doe\", \"url\": \"http://hello.world.doe.com\"}")));
	}

	SECTION("name, email, and url can all be provided at the same time") {
		Person p1(parse("{\"name\": \"John Doe\", \"email\": \"j@doe.com\", \"url\": \"http://doe.com\"}"));
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
