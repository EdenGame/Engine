#include <catch2/catch.hpp>

#include <utilities/version.hpp>

using namespace MUtilities;

TEST_CASE("versions are parsed from a string upon creation", "[version]") {
	SECTION("versions may include only major, minor, and patch data") {
		Version v = Version("1.6.3");
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 6);
		REQUIRE(v.patch() == 3);
	}

	SECTION("versions may include only major, minor, patch, and prerelease data") {
		Version v = Version("1.6.3-rc.2");
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 6);
		REQUIRE(v.patch() == 3);
		REQUIRE(v.prerelease().first == "rc");
		REQUIRE(v.prerelease().second == 2);
	}

	SECTION("versions may include only major, minor, patch, and prerelease type") {
		Version v = Version("1.6.3-rc");
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 6);
		REQUIRE(v.patch() == 3);
		REQUIRE(v.prerelease().first == "rc");
		REQUIRE(v.prerelease().second == -1);
	}

	SECTION("versions may include only major, minor, patch, prerelease type, and build metadata") {
		Version v = Version("1.6.3-rc+buildmetadata");
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 6);
		REQUIRE(v.patch() == 3);
		REQUIRE(v.prerelease().first == "rc");
		REQUIRE(v.prerelease().second == -1);
		REQUIRE(v.meta() == "buildmetadata");
	}

	SECTION("versions may include only major, minor, patch, and build metadata") {
		Version v = Version("1.6.3+buildmetadata");
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 6);
		REQUIRE(v.patch() == 3);
		REQUIRE(v.meta() == "buildmetadata");
	}

	SECTION("versions may include all fields, major, minor, patch, prerelease, and build metadata") {
		Version v = Version("1.6.3-rc.2+buildmetadata");
		REQUIRE(v.major() == 1);
		REQUIRE(v.minor() == 6);
		REQUIRE(v.patch() == 3);
		REQUIRE(v.prerelease().first == "rc");
		REQUIRE(v.prerelease().second == 2);
		REQUIRE(v.meta() == "buildmetadata");
	}

	SECTION("version fields may contain multiple digits") {
		SECTION("versions may include only major, minor, and patch data") {
			Version v = Version("43.12.38");
			REQUIRE(v.major() == 43);
			REQUIRE(v.minor() == 12);
			REQUIRE(v.patch() == 38);
		}

		SECTION("versions may include only major, minor, patch, and prerelease data") {
			Version v = Version("43.12.38-rc.25");
			REQUIRE(v.major() == 43);
			REQUIRE(v.minor() == 12);
			REQUIRE(v.patch() == 38);
			REQUIRE(v.prerelease().first == "rc");
			REQUIRE(v.prerelease().second == 25);
		}

		SECTION("versions may include all fields, major, minor, patch, prerelease, and build metadata") {
			Version v = Version("43.12.38-rc.25+buildmetadata");
			REQUIRE(v.major() == 43);
			REQUIRE(v.minor() == 12);
			REQUIRE(v.patch() == 38);
			REQUIRE(v.prerelease().first == "rc");
			REQUIRE(v.prerelease().second == 25);
			REQUIRE(v.meta() == "buildmetadata");
		}
	}

	SECTION("version numbers may not contain leading digits") {
		REQUIRE_THROWS(Version("01.2.5-rc.5"));
		REQUIRE_THROWS(Version("1.02.5-rc.5"));
		REQUIRE_THROWS(Version("1.2.05-rc.5"));
		REQUIRE_THROWS(Version("1.2.5-rc.05"));
		REQUIRE_NOTHROW(Version("0.1.0-rc.1"));
	}

	SECTION("prerelease version may not be zero, but can be blank") {
		REQUIRE_THROWS(Version("1.6.3-rc.0"));
		Version v = Version("1.6.3-beta");
		REQUIRE(v.prerelease().first == "beta");
		REQUIRE(v.prerelease().second == -1);
	}

	SECTION("version number must be numerical") {
		REQUIRE_THROWS(Version("a.b.c"));
		REQUIRE_THROWS(Version("1.6.3-rc.d"));
	}

	SECTION("build metadata must comprise only ACSII alhpanumerics and hypen") {
		REQUIRE_NOTHROW(Version("1.6.3-rc.1+buildmeta-0.2-to"));
		REQUIRE_THROWS(Version("1.6.3-rc.1+meta&!more"));
	}

	SECTION("versions must always include major, minor, and patch") {
		REQUIRE_THROWS(Version("1.2"));
		REQUIRE_THROWS(Version("1.2-rc"));
		REQUIRE_THROWS(Version("1.2+builtmeta"));
	}
}

TEST_CASE("existing versions can be directly manipulated and read", "[version]") {
	Version v = Version("1.6.3-rc+buildmetadata");

	SECTION("versions can be printed to an ostream directly as a string") {
		std::stringstream ss;
		ss << v;
		REQUIRE(ss.str() == "1.6.3-rc+buildmetadata");
	}

	SECTION("versions objects can be directly assigned a new version string") {
		v = "8.4.7-beta.2+differentbuild";
		REQUIRE(v.major() == 8);
		REQUIRE(v.minor() == 4);
		REQUIRE(v.patch() == 7);
		REQUIRE(v.prerelease().first == "beta");
		REQUIRE(v.prerelease().second == 2);
		REQUIRE(v.meta() == "differentbuild");
	}
}

TEST_CASE("operator overloads compare the main version and prerelease but ignore metadata", "[version]") {
	SECTION("equality operator") {
		REQUIRE(Version("1.2.3") == Version("1.2.3"));
		REQUIRE(Version("1.2.3-rc") == Version("1.2.3-rc"));
		REQUIRE(Version("1.2.3-rc.4") == Version("1.2.3-rc.4"));
		REQUIRE(Version("1.2.3-rc.4+meta.1") == Version("1.2.3-rc.4+meta.2"));
		REQUIRE_FALSE(Version("1.2.3") == Version("3.2.1"));
		REQUIRE_FALSE(Version("1.2.3-rc") == Version("1.2.3"));
		REQUIRE_FALSE(Version("1.2.3-rc.4") == Version("1.2.3-rc"));
		REQUIRE_FALSE(Version("1.2.3-rc.4") == Version("1.2.3-rc.5"));
		REQUIRE_FALSE(Version("1.2.3-alpha") == Version("1.2.3-beta"));

		SECTION("version object equality can be compared with a string") {
			REQUIRE(Version("1.2.3") == "1.2.3");
			REQUIRE_FALSE(Version("1.2.3") == "3.2.1");
		}
	}

	SECTION("inequality is the reverse of equality") {
		REQUIRE_FALSE(Version("1.2.3") != Version("1.2.3"));
		REQUIRE(Version("1.2.3") != Version("3.2.1"));

		SECTION("version object inequality can be compared with a string") {
			REQUIRE_FALSE(Version("1.2.3") != "1.2.3");
			REQUIRE(Version("1.2.3") != "3.2.1");
		}
	}

	SECTION("less than operator") {
		REQUIRE(Version("1.0.0") < Version("2.0.0"));
		REQUIRE(Version("1.0.0") < Version("1.0.1"));
		REQUIRE(Version("1.0.0") < Version("1.1.0"));
		REQUIRE(Version("1.0.1") < Version("1.1.0"));
		REQUIRE(Version("1.0.0-alpha") < Version("1.0.0-alpha.1"));
		REQUIRE(Version("1.0.0-alpha") < Version("1.0.0-beta"));
		REQUIRE(Version("1.0.0-alpha.1") < Version("1.0.0-beta"));
		REQUIRE(Version("1.0.0-beta") < Version("1.0.0-beta.2"));
		REQUIRE(Version("1.0.0-beta.2") < Version("1.0.0-beta.11"));
		REQUIRE(Version("1.0.0-beta.11") < Version("1.0.0-rc.1"));
		REQUIRE(Version("1.0.0-rc.1") < Version("1.0.0"));
		REQUIRE_FALSE(Version("1.0.0-beta") < Version("1.0.0-beta"));

		SECTION("less than operator can compare a version object and a string") {
			REQUIRE(Version("1.0.0") < "2.0.0");
			REQUIRE(Version("1.0.0-alpha.1") < "1.0.0-beta");
			REQUIRE_FALSE(Version("1.0.0-beta") < "1.0.0-beta");
		}
	}

	SECTION("greater than operator is the reverse of the less than operator") {
		REQUIRE(Version("2.0.0") > Version("1.0.0"));
		REQUIRE(Version("1.0.0-alpha.1") > Version("1.0.0-alpha"));
		REQUIRE_FALSE(Version("1.0.0-beta") > Version("1.0.0-beta"));

		SECTION("greater than operator can compare a version object and a string") {
			REQUIRE(Version("2.0.0") > "1.0.0");
			REQUIRE(Version("1.0.0-alpha.1") > "1.0.0-alpha");
			REQUIRE_FALSE(Version("1.0.0-beta") > "1.0.0-beta");
		}
	}

	SECTION("less than or equal to operator") {
		REQUIRE(Version("1.0.0") <= Version("2.0.0"));
		REQUIRE(Version("1.0.0-alpha") <= Version("1.0.0-alpha.1"));
		REQUIRE(Version("1.0.0-alpha.1") <= Version("1.0.0-beta"));
		REQUIRE(Version("1.0.0-beta") <= Version("1.0.0-beta"));

		SECTION("less than or equal to operator can compare a version object and a string") {
			REQUIRE(Version("1.0.0") <= "2.0.0");
			REQUIRE(Version("1.0.0-alpha") <= "1.0.0-alpha.1");
			REQUIRE(Version("1.0.0-beta") <= "1.0.0-beta");
		}
	}

	SECTION("greater than or equal to is the reverse of the less than or equal to operator") {
		REQUIRE(Version("2.0.0") >= Version("1.0.0"));
		REQUIRE(Version("1.0.0-alpha.1") >= Version("1.0.0-alpha"));
		REQUIRE(Version("1.0.0-beta") >= Version("1.0.0-alpha.1"));
		REQUIRE(Version("1.0.0-beta") >= Version("1.0.0-beta"));

		SECTION("greater than or equal to operator can compare a version object and a string") {
			REQUIRE(Version("2.0.0") >= "1.0.0");
			REQUIRE(Version("1.0.0-alpha.1") >= "1.0.0-alpha");
			REQUIRE(Version("1.0.0-beta") >= "1.0.0-beta");
		}
	}
}
