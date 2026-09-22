//
// Yosys slang frontend
//
// This file copyright (c) 2026 Mel Young <mel@mlyoung.cool>
// Distributed under the terms of the ISC license, see LICENSE

// Builder for YosysHQ Property-IR (https://yosyshq.readthedocs.io/projects/property-ir/en/latest;
// https://github.com/YosysHQ/property-ir/)

#include "pir_builder.h"
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

using namespace pir;

namespace {
std::string to_string(const Evaluate &evaluate)
{
	switch (evaluate) {
	case Evaluate::EvaluateAlways:  return "always";
	case Evaluate::EvaluateInitial: return "initial";
	}
	return "pir_builder_internal_error";
}

std::string to_string(const CoverMode &mode)
{
	switch (mode) {
	case CoverMode::Satisfied:             return "satisfied";
	case CoverMode::NonvacuouslySatisfied: return "nonvacuously-satisfied";
	case CoverMode::Nonvacuous:            return "nonvacuous";
	}
	return "pir_builder_internal_error";
}
}; // namespace

DeclareStatement::DeclareStatement(std::string identifier, Expression::Ptr expression)
	: identifier(std::move(identifier)), expr(std::move(expression))
{}

void DeclareStatement::build(std::stringstream &stream)
{
	stream << "(declare " << identifier << " ";
	expr->build(stream);
	stream << ")";
}

void AssertAssumeRestrictPropertyStatement::build(std::stringstream &stream)
{
	stream << "(";

	switch (method) {
	case PropertyMethod::Assert:   stream << "assert-"; break;
	case PropertyMethod::Assume:   stream << "assume-"; break;
	case PropertyMethod::Restrict: stream << "restrict-"; break;
	}

	stream << "property " << clk_prop;
	// we always want boolalpha, so no need to turn it off
	stream << std::boolalpha;

	if (disable_iff != std::nullopt) {
		stream << " :disable-iff " << *disable_iff;
	}

	if (enable != std::nullopt) {
		stream << " :enable " << *enable;
	}

	if (evaluate != std::nullopt) {
		stream << " :evaluate " << to_string(*evaluate);
	}

	stream << ")";
}

void CoverStatement::build(std::stringstream &stream)
{
	stream << "(cover-";

	switch (kind) {
	case CoverKind::Property: stream << "property"; break;
	case CoverKind::Sequence: stream << "sequence"; break;
	}

	stream << " " << clk_prop;

	// we always want boolalpha, so no need to turn it off
	stream << std::boolalpha;

	if (disable_iff != std::nullopt) {
		stream << " :disable-iff " << *disable_iff;
	}

	if (enable != std::nullopt) {
		stream << " :enable " << *enable;
	}

	if (evaluate != std::nullopt) {
		stream << " :evaluate " << to_string(*evaluate);
	}

	if (mode != std::nullopt) {
		stream << " :mode " << to_string(*mode);
	}

	stream << ")";
}
