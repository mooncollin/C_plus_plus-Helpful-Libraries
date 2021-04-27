#pragma once

namespace cmoon::cuda
{
	template<class T>
	void plus_ranges_impl(int, const T* const, const T* const, T*) noexcept;
}