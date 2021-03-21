#pragma once

#include <type_traits>
#include <future>

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class T>
			struct is_future : std::false_type {};

			template<class T>
			struct is_future<std::future<T>> : std::true_type {};

			template<class T>
			constexpr auto is_future_v = is_future<T>::value;
		}
	}
}