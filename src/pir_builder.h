//
// Yosys slang frontend
//
// This file copyright (c) 2026 Mel Young <mel@mlyoung.cool>
// Distributed under the terms of the ISC license, see LICENSE

// Builder for YosysHQ Property-IR (https://yosyshq.readthedocs.io/projects/property-ir/en/latest;
// https://github.com/YosysHQ/property-ir/)
#pragma once
#include <memory>
#include <optional>
#include <sstream>
#include <string>

namespace pir {

/// Root of the Property IR hierarchy
struct Type
{
	/// Method to turn this node into a string. Subclasses should override.
	virtual void build(std::stringstream &stream) = 0;

	using Ptr = std::shared_ptr<Type>;
};

struct Expression : public Type
{
	using Ptr = std::shared_ptr<Expression>;
};

struct Statement : public Type
{
	using Ptr = std::shared_ptr<Statement>;
};

struct DeclareStatement : public Statement
{
	DeclareStatement(const std::string &identifier, const Expression::Ptr &expression);

	void build(std::stringstream &stream);

private:
	std::string identifier;
	Expression::Ptr expr;
};

/// The evaluate argument. This is not in and of itself a type.
enum Evaluate {
	EvaluateAlways,
	EvaluateInitial,
};

enum CoverMode {
	Satisfied,
	NonvacuouslySatisfied,
	Nonvacuous,
};

enum CoverKind {
	Property,
	Sequence,
};

enum PropertyMethod { Assert, Assume, Restrict };

struct AssertAssumeRestrictPropertyStatement : public Statement
{
	AssertAssumeRestrictPropertyStatement(const PropertyMethod &method, const std::string &clk_prop,
			const std::optional<bool> disable_iff = std::nullopt,
			const std::optional<bool> enable = std::nullopt,
			const std::optional<Evaluate> &evaluate = std::nullopt)
		: method(method), clk_prop(clk_prop), disable_iff(disable_iff), enable(enable),
		  evaluate(evaluate)
	{}

	void build(std::stringstream &stream);

private:
	const PropertyMethod method;
	const std::string &clk_prop;
	const std::optional<bool> disable_iff;
	const std::optional<bool> enable;
	const std::optional<Evaluate> evaluate;
};

struct CoverStatement : public Statement
{
	CoverStatement(const CoverKind &kind, const std::string &clk_prop,
			const std::optional<bool> disable_iff = std::nullopt,
			const std::optional<bool> enable = std::nullopt,
			const std::optional<Evaluate> &evaluate = std::nullopt,
			const std::optional<CoverMode> &mode = std::nullopt)
		: kind(kind), clk_prop(clk_prop), disable_iff(disable_iff), enable(enable),
		  evaluate(evaluate), mode(mode)
	{}

	void build(std::stringstream &stream);

private:
	const CoverKind kind;
	const std::string &clk_prop;
	const std::optional<bool> disable_iff;
	const std::optional<bool> enable;
	const std::optional<Evaluate> evaluate;
	const std::optional<CoverMode> mode;
};

}; // namespace pir
