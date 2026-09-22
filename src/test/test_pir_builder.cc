//
// Yosys slang frontend
//
// This file copyright (c) 2026 Mel Young <mel@mlyoung.cool>
// Distributed under the terms of the ISC license, see LICENSE

// Tests for Property IR builder

#include "pir_builder.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>

using namespace pir;

TEST_CASE("assert-property", "pir-builder")
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

TEST_CASE("assume-property", "pir-builder")
{
	AssertAssumeRestrictPropertyStatement statement(PropertyMethod::Assume, "prop");
	std::stringstream s;
	statement.build(s);
	std::string str = s.str();
	REQUIRE(str == "(assume-property prop)");
}

TEST_CASE("restrict-property", "pir-builder")
{
	AssertAssumeRestrictPropertyStatement statement(PropertyMethod::Restrict, "prop");
	std::stringstream s;
	statement.build(s);
	std::string str = s.str();
	REQUIRE(str == "(restrict-property prop)");
}

TEST_CASE("cover-property", "pir-builder")
{
	{
		CoverStatement statement(CoverKind::Property, "prop");
		std::stringstream s;
		statement.build(s);
		REQUIRE(s.str() == "(cover-property prop)");
	}

	{
		CoverStatement statement(CoverKind::Property, "prop", true, false, Evaluate::EvaluateAlways,
				CoverMode::Nonvacuous);
		std::stringstream s;
		statement.build(s);
		REQUIRE(s.str() == "(cover-property prop :disable-iff true :enable false :evaluate always "
						   ":mode nonvacuous)");
	}

	{
		CoverStatement statement(CoverKind::Property, "prop", std::nullopt, false, std::nullopt,
				CoverMode::Nonvacuous);
		std::stringstream s;
		statement.build(s);
		REQUIRE(s.str() == "(cover-property prop :enable false :mode nonvacuous)");
	}
}

TEST_CASE("cover-sequence", "pir-builder")
{
	CoverStatement statement(CoverKind::Sequence, "prop");
	std::stringstream s;
	statement.build(s);
	REQUIRE(s.str() == "(cover-sequence prop)");
}
