#pragma once
#include <map>
#include <memory>

#include "IStringID.h"

namespace AnimationSystem
{
	/**
	 * \brief Wrapper for implementation-specific objects. Manual ownership is strictly forbidden through this interface.
	 * Attempting to create or delete an object via a pointer of this type is a compile-time error,
	 * and a raw pointer to this object never implies ownership.
	 * \tparam T The class contained in this wrapper. Accessible through IProtectedWrapper<T>::Item().
	 */
	template <class T>
	class ViewingProtectedWrapper final : public IStringID
	{
	public:
		typedef ViewingProtectedWrapper<T> self;
		typedef std::unique_ptr<self> unique_ptr_to_self;
		
		static unique_ptr_to_self Create(T const & item, const unsigned id)
		{
			auto itemPtr = unique_ptr_to_self{ new self{std::move(item), id}};
			return itemPtr;
		}
		
		T const & Item() const {return item_;}
		unsigned ID() const override {return id_;}

		ViewingProtectedWrapper(const ViewingProtectedWrapper& other) = delete;
		ViewingProtectedWrapper(ViewingProtectedWrapper&& other) noexcept = delete;
		ViewingProtectedWrapper& operator=(const ViewingProtectedWrapper& other)= delete;
		ViewingProtectedWrapper& operator=(ViewingProtectedWrapper&& other) noexcept= delete;
	private:
		// prevent creation outside of class
		ViewingProtectedWrapper(T const & item, const unsigned id)
			:item_(item)
			,id_(id)
		{
		}
		~ViewingProtectedWrapper() override = default;
		
		T const & item_;
		const unsigned id_;
		
		void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

		friend struct std::default_delete<ViewingProtectedWrapper<T>>; // allow deletion by smart pointer
	};


	/**
	 * \brief Wrapper for implementation-specific objects. Manual ownership is strictly forbidden through this interface.
	 * Attempting to create or delete an object via a pointer of this type is a compile-time error,
	 * and a raw pointer to this object never implies ownership.
	 * \tparam T The class contained in this wrapper. Accessible through IProtectedWrapper<T>::Item().
	 */
	template <class T>
	class OwningProtectedWrapper final: public IStringID
	{
	public:
		typedef OwningProtectedWrapper<T> self;
		typedef std::unique_ptr<self> unique_ptr_to_self;
		
		static unique_ptr_to_self Create(std::unique_ptr<T> item, const unsigned id)
		{
			auto itemPtr = unique_ptr_to_self{ new self{std::move(item), id}};
			return itemPtr;
		}
		static unique_ptr_to_self Create(const unsigned id)
		{
			return Create(std::move(std::make_unique<T>()), id);
		}

		template<typename... Args>
		static unique_ptr_to_self Create(const unsigned id, Args&& ...args)
		{
			return Create(std::move(std::make_unique<T>(args...)), id);
		}
		
		T & Item() const {return *item_;}
		unsigned ID() const override {return id_;}
		
		OwningProtectedWrapper(const OwningProtectedWrapper& other) = delete;
		OwningProtectedWrapper(OwningProtectedWrapper&& other) noexcept = delete;
		OwningProtectedWrapper& operator=(const OwningProtectedWrapper& other)= delete;
		OwningProtectedWrapper& operator=(OwningProtectedWrapper&& other) noexcept= delete;
	private:
		// prevent creation outside of class
		OwningProtectedWrapper(std::unique_ptr<T> item, const unsigned id)
			:item_(std::move(item))
			,id_(id)
		{
		}
		~OwningProtectedWrapper() override = default;
		
		std::unique_ptr<T> item_;
		const unsigned id_;
		
		void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

		friend struct std::default_delete<OwningProtectedWrapper<T>>; // allow deletion by smart pointer
	};

	template <class T>
	T * GetWrappedValueFromMap(std::map<unsigned, std::unique_ptr<T>> const & map, unsigned key)
	{
		const auto iter = map.find(key);
		if(iter == map.end())
			return nullptr;
		return iter->second.get();
	}
	
}
