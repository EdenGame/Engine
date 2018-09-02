#include <catch2/catch.hpp>
#include <iostream>

#include <json/json.h>

#include <utilities/utility.hpp>

using namespace MUtilities::Utility;

TEST_CASE("a string is parsed to a json value", "[utility]") {
	Json::Value obj = stojson(R"({"key": "value", "bool": true, "arr": ["one", "two"], "int": 11})");
	// Check key: value
	REQUIRE(obj["key"].isString());
	REQUIRE(obj["key"].asString() == "value");
	// Check bool: true
	REQUIRE(obj["bool"].isBool());
	REQUIRE(obj["bool"].asBool() == true);
	// Check arr: [one, two]
	REQUIRE(obj["arr"].isArray());
	REQUIRE(obj["arr"][0].isString());
	REQUIRE(obj["arr"][0].asString() == "one");
	REQUIRE(obj["arr"][1].isString());
	REQUIRE(obj["arr"][1].asString() == "two");
	// Check int: 11
	REQUIRE(obj["int"].isInt());
	REQUIRE(obj["int"].asInt() == 11);
}

TEST_CASE("check if a string ends with the provided suffix", "[utility]") {
	REQUIRE_FALSE(hasSuffix("no", ".git"));
	REQUIRE_FALSE(hasSuffix("verylong.com", ".git"));
	REQUIRE(hasSuffix("https://doe.com/hello.git", ".git"));
}
