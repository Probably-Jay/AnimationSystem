#pragma once
#include <memory>

namespace AnimationSystem
{
	/**
	 * \brief Wrapper for implementation-specific objects. Manual ownership is strictly forbidden through this interface.
	 * Attempting to create or delete an object via a pointer of this type is a compile-time error,
	 * and a raw pointer to this object never implies ownership.
	 * \tparam T The class contained in this wrapper. Accessible through IProtectedWrapper<T>::Item().
	 */
	template <class T>
	class ViewingProtectedWrapper
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
		unsigned ID() const {return id_;}
		
	private:
		// prevent creation outside of class
		ViewingProtectedWrapper(T const & item, const unsigned id)
			:item_(item)
			,id_(id)
		{
		}

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
	class OwningProtectedWrapper
	{
	public:
		typedef OwningProtectedWrapper<T> self;
		typedef std::unique_ptr<self> unique_ptr_to_self;
		
		static unique_ptr_to_self Create(std::unique_ptr<T> item, const unsigned id)
		{
			auto itemPtr = unique_ptr_to_self{ new self{std::move(item), id}};
			return itemPtr;
		}
		
		T & Item() const {return *item_;}
		unsigned ID() const {return id_;}
		
	private:
		// prevent creation outside of class
		OwningProtectedWrapper(std::unique_ptr<T> item, const unsigned id)
			:item_(std::move(item))
			,id_(id)
		{
		}

	
		std::unique_ptr<T> item_;
		const unsigned id_;
		
		void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

		friend struct std::default_delete<OwningProtectedWrapper<T>>; // allow deletion by smart pointer
	};

	
}
