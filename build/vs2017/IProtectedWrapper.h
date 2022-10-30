#pragma once
#include <memory>

namespace AnimationSystem
{
	/**
	 * \brief Wrapper for implementation-specific objects. Manual ownership is strictly forbidden through this interface.
	 * Instead, deriving types must offer a static factory method returning a unique_pointer of this type.
	 * Attempting to create or delete an object via a pointer of this type is a compile-time error.
	 * \tparam T The class contained in this wrapper. Accessible through IProtectedWrapper<T>::Item().
	 */
	template <class T>
	class IProtectedWrapper
	{
	public:

		static std::unique_ptr<IProtectedWrapper<T>> Create(T const & item, const unsigned id)
		{
			auto itemPtr = std::unique_ptr<IProtectedWrapper<T>>{ new IProtectedWrapper<T>{item, id}};
			return itemPtr;
		}

		
		T const & Item() const {return item_;}
		unsigned ID() const {return id_;}
		
	protected:
		// prevent creation outside of class
		IProtectedWrapper(T const & item, const unsigned id)
			:item_(item)
			,id_(id)
		{
		}

	private:

		T const & item_;
		const unsigned id_;
		
		void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

		friend struct std::default_delete<IProtectedWrapper>; // allow deletion by smart pointer
	};
}
