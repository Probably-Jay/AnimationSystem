#pragma once
#include <memory>

namespace AnimationSystem
{
	/**
	 * \brief Wrapper for implementation-specific objects. Ownership is strictly forbidden through this interface.
	 * Attempting to create or destroy an object via a pointer of this type is a compile-time error.
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
