module;

#pragma warning(disable : 4005)
#pragma warning(disable : 5105)
#pragma warning(disable : 5106)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.global_lock;

import cmoon.scope;

namespace cmoon::windows
{
	template<class T>
	struct global_lock_default_deleter
	{
		void operator()(T* ptr) const noexcept
		{
			::GlobalUnlock(static_cast<::HGLOBAL>(ptr));
		}
	};

	export
	template<class T>
	using global_lock_resource = cmoon::unique_resource<T*, global_lock_default_deleter<T>>;

	export
	template<class T>
	[[nodiscard]] global_lock_resource<T> global_resource(T* ptr) noexcept
	{
		return cmoon::make_unique_resource_checked(
			static_cast<T*>(::GlobalLock(static_cast<::HGLOBAL>(ptr))),
			nullptr,
			global_lock_default_deleter<T>{}
		);
	}
}