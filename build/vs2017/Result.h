#pragma once
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

#include "IStringID.h"
#define ERROR_TAG ("Unhandled error result in: " + std::string(__FUNCTION__) + ", Message: ")  
namespace AnimationSystem
{
	struct Result
	{
		
		static Result OK() { return Result{ }; }
		static Result Error(std::string	error) { return Result{ (std::move(error)) }; }
	
		bool Successful() const {return successful_;}
		bool IsError() const {return !Successful();}
		std::string ErrorMessage() const {return error_message_;}

		Result(const Result& other) = default;

		Result(Result&& other) noexcept
			: successful_(other.successful_),
			  error_message_(std::move(other.error_message_))
		{
			other.Handled();
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
			other.Handled();
			return *this;
		}

		void Handled() const {handled_ = true;}
		
		// todo add result handling?
		~Result() noexcept(false)
		{
			if(IsError() && !handled_)
				throw std::runtime_error(error_message_);
		}
		
	protected:
		Result() :successful_(true), error_message_(std::string()) {}
		explicit Result(std::string error) :successful_(false), error_message_(std::move(error)) {}

		bool successful_;
		std::string error_message_;

		mutable bool handled_ = false;
	};

	struct CreateEntityResult : public Result
	{
	public:
		static CreateEntityResult OK(const StringId id) { return CreateEntityResult{ id }; }
		static CreateEntityResult Error(std::string	error) { return CreateEntityResult{ std::move(error) }; }
		static CreateEntityResult Error(const Result& error) { return CreateEntityResult{ error.ErrorMessage() }; }

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
