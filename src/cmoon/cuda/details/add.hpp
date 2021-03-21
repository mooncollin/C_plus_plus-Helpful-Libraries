#pragma once

namespace cmoon
{
	namespace cuda
	{
		template<class T>
		void plus_ranges_impl(int, const T* const, const T* const, T*);
	}
}