#pragma once
#include <algorithm>
#include <string>

struct Result
{
	static Result OK() { return Result{ }; }
	static Result Error(std::string	error) { return Result{ std::move(error) }; }
	bool Successful() const {return successful_;}
	bool IsError() const {return !Successful();}
	std::string ErrorMessage() const {return error_message_;}

	Result(const Result& other) = default;

	Result(Result&& other) noexcept
		: successful_(other.successful_),
		  error_message_(std::move(other.error_message_))
	{
	}

	Result& operator=(const Result& other)
	{
		if (this == &other)
			return *this;
		successful_ = other.successful_;
		error_message_ = other.error_message_;
		return *this;
	}

	Result& operator=(Result&& other) noexcept
	{
		if (this == &other)
			return *this;
		successful_ = other.successful_;
		error_message_ = std::move(other.error_message_);
		return *this;
	}

	~Result() = default;
private:
	Result() :successful_(true), error_message_(std::string()) {}
	explicit Result(std::string error) :successful_(false), error_message_(std::move(error)) {}

	bool successful_;
	std::string error_message_;
};

