#include <catch2/catch.hpp>
#include <iostream>

#include <utilities/utility.hpp>
#include <fstream>

#include <packages/package.hpp>

using namespace MPackages;

class Pkg: public Package {
	public:
		Pkg(std::string path, Json::Value obj): Package(path, obj) {}

		bool run() {
			std::cout << "Called run!" << std::endl;
		}
};

TEST_CASE("package objects are generated based off of json data", "[package]") {
	SECTION("creating a package object requires that the name and version be specified") {
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(R"({"title": "Hello World!"})")));
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(R"({"name": "package"})")));
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(R"({"version": "0.1.0"})")));
		REQUIRE_NOTHROW(Pkg("", MUtilities::Utility::stojson(R"({"name": "package", "version": "0.1.0"})")));
	}

	SECTION("package name may only contain certain characters") {
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(R"({"name": "_package", "version": "0.1.0"})")));
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(R"({"name": "pAckage", "version": "0.1.0"})")));
		REQUIRE_NOTHROW(Pkg("", MUtilities::Utility::stojson(R"({"name": "p_c-9", "version": "0.1.0"})")));
	}

	SECTION("homepage url must be a valid uri") {
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "homepage": "doe.com"})")));
		REQUIRE_NOTHROW(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "homepage": "https://www.doe.com/hi"})")));
	}

	SECTION("bugs email and url must be valid") {
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "bugs": {"email": "invalid"}})")));
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "bugs": {"url": "doe.com"}})")));
		REQUIRE_NOTHROW(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "bugs": {"email": "j@doe.com", "url": "https://doe.com"}})")));
	}

	SECTION("repository must be a valid url ending with .git") {
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "repository": "doe.com"})")));
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "repository": "https://doe.com"})")));
		REQUIRE_NOTHROW(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "repository": "https://doe.com/hello.git"})")));
	}

	SECTION("dependency names must include valid package type identifier, package names, and version ranges") {
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "dependencies": {"invalid:pkg": "0.1.0"}})")));
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "dependencies": {"collection:_pkg": "0.1.0"}})")));
		REQUIRE_THROWS(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "dependencies": {"ssm:pkg": "0.1.0-rc.0"}})")));
		REQUIRE_NOTHROW(Pkg("", MUtilities::Utility::stojson(
			R"({"name": "pkg", "version": "0.1.0", "dependencies": {"ssm:pkg": "0.1.0"}})")));
	}

	SECTION("all fields are parsed accurately") {
		std::string str = R"r({"name": "pkg", "version": "0.1.0", "title": "Package",)r"
			R"r("description": "My Package", "keywords": ["one", "two"], "homepage": "https://doe.com",)r"
			R"r("bugs": {"url": "https://doe.com", "email": "j@doe.com"}, "license": "MIT",)r"
			R"r("author": "John Doe <j@doe.com> (https://doe.com)", "contributors": ["Johnny Doe", "Johnson Doe"],)r"
			R"r("repository": "https://doe.com/repo.git", "dependencies": {"ssm:one": ">1.2.6",)r"
			R"r("csm:two": ">3.1.7"}, "optionalDependencies": {"res:another": ">0.1.0"}, "private": false})r";
		Pkg pkg("", MUtilities::Utility::stojson(str));

		REQUIRE(pkg.name() == "pkg");
		REQUIRE(pkg.version() == "0.1.0");
		REQUIRE(pkg.title() == "Package");
		REQUIRE(pkg.description() == "My Package");
		REQUIRE(pkg.keywords()[0] == "one");
		REQUIRE(pkg.keywords()[1] == "two");
		REQUIRE(pkg.homepage() == "https://doe.com");
		REQUIRE(pkg.bugs().first == "https://doe.com");
		REQUIRE(pkg.bugs().second == "j@doe.com");
		REQUIRE(pkg.license() == "MIT");
		REQUIRE(pkg.author() == std::string("John Doe <j@doe.com> (https://doe.com)"));
		REQUIRE(pkg.contributors()[0] == std::string("Johnny Doe"));
		REQUIRE(pkg.contributors()[1] == std::string("Johnson Doe"));
		REQUIRE(pkg.repository() == "https://doe.com/repo.git");
		REQUIRE(pkg.dependencies().at("ssm:one") == ">1.2.6");
		REQUIRE(pkg.dependencies().at("csm:two") == ">3.1.7");
		REQUIRE(pkg.optionalDependencies().at("res:another") == ">0.1.0");
		REQUIRE(pkg.isPrivate() == false);
	}
}
