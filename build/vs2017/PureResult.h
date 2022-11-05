#pragma once
#include <algorithm>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include "IStringID.h"
#include "SkinnedMeshWrapper.h"
#define ERROR_TAG ("Unhandled error in: " + std::string(__FUNCTION__) + ", Message: ")  
namespace AnimationSystem
{
	struct ErrorStruct
	{
		ErrorStruct(std::string message) :error_message_(std::move(message)){  }
		std::string Message() const {return error_message_;}
		void HandleError() {handled_ = true;}
		bool UnHandled() const {return !handled_;}
	private:
		bool handled_ = false;
		std::string error_message_;
	};


	struct PureResult
	{
		static PureResult OK() { return PureResult{ }; }
		static PureResult Error(std::string	error) { return PureResult{ (std::move(error)) }; }
		static PureResult Error(ErrorStruct error) { return PureResult{ std::move(error) }; }
		static PureResult Error(const std::exception& error) {return PureResult{error.what()};}
		static PureResult Error(const std::string& error, const std::exception& exception) {return PureResult{error + exception.what()};}

		bool IsError() const {return error_.has_value();}

		std::optional<ErrorStruct> Error() const{return error_;}

		void HandleError() {error_.reset();}


		PureResult(const PureResult& other)=default;

		PureResult(PureResult&& other) noexcept
			: error_(std::move(other.error_))
		{
			other.HandleError();
		}


		PureResult& operator=(const PureResult& other)
		{
			if (this == &other)
				return *this;
			error_ = other.error_;
			return *this;
		}

		PureResult& operator=(PureResult&& other) noexcept
		{
			if (this == &other)
				return *this;
			error_ = std::move(other.error_);
			other.HandleError();
			return *this;
		}

		// todo add result handling?
		~PureResult() noexcept(false)
		{	
			if(IsError() && Error().value().UnHandled() )
			{
				const auto message = Error().value().Message();
				std::cout << message << std::endl;
				throw std::runtime_error(message);
			}
		}
		
	protected:
		PureResult() : error_({}) {}
		explicit PureResult(std::string error) : error_(ErrorStruct(std::move(error))) {}
		explicit PureResult(const ErrorStruct error) : error_(error){}

		std::optional<ErrorStruct> error_;
	};

	template <class T>
	struct ValueResult
	{
	public:
	//	static ValueResult OK(T entity) { return ValueResult{ entity }; }
		static ValueResult OK(T && entity) { return ValueResult{ std::move(entity) }; }
		static ValueResult Error(std::string error) { return ValueResult{ std::move(error) }; }
		static ValueResult Error(ErrorStruct error) { return ValueResult{ std::move(error) }; }

		bool IsError() const {return std::holds_alternative<ErrorStruct>(result_);}
		std::optional<ErrorStruct> Error() const
		{
			if (IsError())
				return std::get<ErrorStruct>(result_);
			return {};
		}
		
		PureResult ToPureResult() const
		{
			return IsError() ? PureResult::Error(Error().value()) : PureResult::OK();
		}
		
		void HandleError() const
		{
			if(!IsError())
				return;
			Error().value().HandleError();
		}

		std::optional<T> TryGet() const
		{
			if (std::holds_alternative<T>(result_))
				return std::get<T>(result_);
			return {};
		}

		T Get() const
		{
			return std::get<T>(result_);
		}

		T Take()
		{
			return std::move(std::get<T>(result_));
		}

	ValueResult(const ValueResult& other)
			: result_(other.result_)
		{
		}

		ValueResult(ValueResult&& other) noexcept
			: result_(std::move(other.result_))
		{
			other.HandleError();
		}

		ValueResult& operator=(const ValueResult& other)
		{
			if (this == &other)
				return *this;
			result_ = other.result_;
			return *this;
		}

		ValueResult& operator=(ValueResult&& other) noexcept
		{
			if (this == &other)
				return *this;
			other.HandleError();
			result_ = std::move(other.result_);
			return *this;
		}

		~ValueResult() noexcept(false)
		{
			if(IsError() && Error().value().UnHandled())
			{
				const auto message = Error().value().Message();
				std::cout << message << std::endl;
				throw std::runtime_error(message);
			}
		}

	private:
		ValueResult(T entity)
			: result_(std::move(entity))
		{
		}

		ValueResult(std::string error)
			: result_(ErrorStruct{std::move(error)})
		{
		}

		ValueResult(ErrorStruct error)
			: result_(error)
		{
		}
		
		std::variant<T, ErrorStruct> result_;
	};


	template <class T>
	ValueResult<std::reference_wrapper<T>> TryGetValueFromMap(std::map<unsigned, std::unique_ptr<T>> const & map, unsigned key)
	{
		const auto iter = map.find(key);
		if(iter == map.end())
			return ValueResult<std::reference_wrapper<T>>::Error(ERROR_TAG+"Could not find value in map");
		return ValueResult<std::reference_wrapper<T>>::OK(*iter->second);
	}
	
}
