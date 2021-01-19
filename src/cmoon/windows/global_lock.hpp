#pragma once

#include <windows.h>

#include "cmoon/scope.hpp"

namespace cmoon
{
	namespace windows
	{

		namespace details
		{
			template<class T>
			struct global_lock_default_deleter
			{
				void operator()(T* ptr) const noexcept
				{
					::GlobalUnlock(static_cast<::HGLOBAL>(ptr));
				}
			};

			template<class T>
			using global_lock_resource = cmoon::unique_resource<T*, global_lock_default_deleter<T>>;
		}

		template<class T>
		[[nodiscard]] details::global_lock_resource<T> global_resource(T* ptr) noexcept
		{
			return cmoon::make_unique_resource_checked(
				static_cast<T*>(::GlobalLock(static_cast<::HGLOBAL>(ptr))),
				nullptr,
				details::global_lock_default_deleter<T>{}
			);
		}
	}
}