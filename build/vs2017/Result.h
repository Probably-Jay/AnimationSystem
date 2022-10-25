#pragma once
#include <algorithm>
#include <string>

struct Result
{
	static Result OK() { return Result{ }; }
	static Result Error(std::string	error) { return Result{ std::move(error) }; }
	const bool Successful;
	const std::string errorMessage;
private:
	Result() :Successful(true), errorMessage(std::string()) {}
	explicit Result(std::string error) :Successful(false), errorMessage(std::move(error)) {}
};

