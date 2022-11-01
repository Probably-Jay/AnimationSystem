#pragma once
#include <algorithm>
#include <string>
#include <utility>

#include "IStringID.h"

namespace AnimationSystem
{
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

		// todo add result handling?
		~Result() = default;
		
	protected:
		Result() :successful_(true), error_message_(std::string()) {}
		explicit Result(std::string error) :successful_(false), error_message_(std::move(error)) {}

		bool successful_;
		std::string error_message_;
	};

	struct CreateEntityResult : public Result
	{
		
		static CreateEntityResult OK(const StringId id) { return CreateEntityResult{ id }; }
		static CreateEntityResult Error(std::string	error) { return CreateEntityResult{ std::move(error) }; }

		StringId EntityID() const {return createdEntityId;}
	private:
		CreateEntityResult(StringId createdEntityId)
			: Result(),
			  createdEntityId(createdEntityId)
		{
		}

		CreateEntityResult(std::string error)
			: Result(std::move(error)),
			  createdEntityId(NULL)
		{
		}

		StringId createdEntityId;
	};
}
