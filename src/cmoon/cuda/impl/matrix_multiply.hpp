#pragma once

namespace cmoon::cuda
{
	template<class T>
	void matrix_multiply_impl(int m, int n, int k, const T* const input1, const T* const input2, T* out) noexcept;
}