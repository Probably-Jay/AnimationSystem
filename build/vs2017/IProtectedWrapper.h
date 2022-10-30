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
		virtual ~IProtectedWrapper() = default;
		virtual T const & Item() const = 0;
		virtual unsigned ID() const = 0;
		
	protected:
		IProtectedWrapper() = default; // prevent creation outside of class

	private:
		void operator delete(void * p){::operator delete(p);} // prevent deletion outside of class

		friend struct std::default_delete<IProtectedWrapper>; // allow deletion by smart pointer
	};
}
