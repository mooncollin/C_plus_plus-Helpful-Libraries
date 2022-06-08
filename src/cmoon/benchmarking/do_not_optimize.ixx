export module cmoon.benchmarking.do_not_optimize;

import <memory>;

namespace cmoon::benchmarking
{
	void use_char_pointer(const volatile char*) noexcept {}

	export
	template<class T>
	inline void do_not_optimize(const T& value) noexcept
	{
		use_char_pointer(&reinterpret_cast<const volatile char&>(value));
		_ReadWriteBarrier();
	}

	/*export
	template<class T>
	inline void do_not_optimize(T&& value) noexcept
	{
		auto non_optimized_pointer {std::addressof(reinterpret_cast<volatile T&>(value))};
		use_pointer(reinterpret_cast<volatile char*>(non_optimized_pointer));
	}*/
}