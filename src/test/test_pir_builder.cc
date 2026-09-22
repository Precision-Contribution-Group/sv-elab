#include "pir_builder.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>

using namespace pir;

TEST_CASE("PIR assert-property", "[pir-builder]")
{
	{
		AssertAssumeRestrictPropertyStatement statement(PropertyMethod::Assert, "prop");
		std::stringstream s;
		statement.build(s);
		std::string str = s.str();
		REQUIRE(str == "(assert-property prop)");
	}

	{
		AssertAssumeRestrictPropertyStatement statement(
				PropertyMethod::Assert, "prop", true, true, Evaluate::EvaluateAlways);
		std::stringstream s;
		statement.build(s);
		std::string str = s.str();
		REQUIRE(str == "(assert-property prop :disable-iff true :enable true :evaluate always)");
	}

	{
		AssertAssumeRestrictPropertyStatement statement(
				PropertyMethod::Assert, "prop", std::nullopt, true, Evaluate::EvaluateAlways);
		std::stringstream s;
		statement.build(s);
		std::string str = s.str();
		REQUIRE(str == "(assert-property prop :enable true :evaluate always)");
	}
}
