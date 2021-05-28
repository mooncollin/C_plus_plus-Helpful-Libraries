export module cmoon.benchmark.do_not_optimize;

import <memory>;

import cmoon.platform;

namespace cmoon::benchmark
{
	void use_char_pointer(const volatile char*) {}

	export
	template<class T>
	inline void do_not_optimize(const T& value)
	{
		use_char_pointer(std::addressof(reinterpret_cast<const volatile char&>(value)));
	}
}