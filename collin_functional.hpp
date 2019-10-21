#ifndef COLLIN_FUNCTIONAL
#define COLLIN_FUNCTIONAL

#include <functional>

namespace collin
{
	template<class T>
	auto equal_bind(T thing)
	{
		const auto obj = std::equal_to<T>();
		const auto mem = std::mem_fn(&std::equal_to<T>::operator());
		return std::bind(mem, obj, std::placeholders::_1, thing);
	}
}

#endif