//
// Yosys slang frontend
//
// This file copyright (c) 2026 Mel Young <mel@mlyoung.cool>
// Distributed under the terms of the ISC license, see LICENSE

// Builder for YosysHQ Property-IR (https://yosyshq.readthedocs.io/projects/property-ir/en/latest;
// https://github.com/YosysHQ/property-ir/)

#include "pir_builder.h"
#include <optional>
#include <sstream>
#include <string>

using namespace pir;

std::string to_string(const Evaluate &evaluate)
{
	switch (evaluate) {
	case EvaluateAlways:  return "always";
	case EvaluateInitial: return "initial";
	}
	return "pir_builder_internal_error";
}

std::string to_string(const CoverMode &mode)
{
	switch (mode) {
	case Satisfied:             return "satisfied";
	case NonvacuouslySatisfied: return "nonvacuously-satisfied";
	case Nonvacuous:            return "nonvacuous";
	}
	return "pir_builder_internal_error";
}

Declare::Declare(const std::string &identifier, const Expression::Ptr &expression)
{
	this->identifier = identifier;
	this->expr = expression;
}

void AssertAssumeRestrictProperty::build(std::stringstream &stream)
{
	stream << "(";

	switch (method) {
	case Assert:   stream << "assert-"; break;
	case Assume:   stream << "assume-"; break;
	case Restrict: stream << "restrict-"; break;
	}

	stream << "property " << clk_prop;

	if (disable_iff != std::nullopt) {
		stream << " :disable-iff " << *disable_iff;
	}

	if (enable != std::nullopt) {
		stream << " :enable " << *enable;
	}

	if (evaluate != std::nullopt) {
		stream << " :evaluate " << to_string(*evaluate);
	}
}
