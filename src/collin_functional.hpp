#ifndef COLLIN_FUNCTIONAL
#define COLLIN_FUNCTIONAL

#include <functional>
#include <utility>
#include <tuple>

namespace collin
{
	template<class... I, class Function, class... T>
	auto bind_multiple(Function func, I... placeholders, T... things)
	{
		const auto mem = std::mem_fn(&Function::operator());
		return std::bind(mem, func, (std::forward<I>(placeholders))..., (std::forward<T>(things))...);
	}

	template<class T, class Function>
	auto bind_1(Function func, T thing)
	{
		return bind_multiple(func, std::placeholders::_1, std::move(thing));
	}

	template<class T, class Function>
	auto bind_2(Function func, T thing)
	{
		return bind_multiple(func, std::placeholders::_1, std::placeholders::_2, std::move(thing));
	}

	template<class T, class Function>
	auto bind_3(Function func, T thing)
	{
		return bind_multiple(func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::move(thing));
	}

	template<class T>
	auto equal_bind(T thing)
	{
		return bind_1(std::equal_to<T>(), std::move(thing));
	}

	struct pair_hash
	{
		template<class T1, class T2>
		std::size_t operator()(const std::pair<T1, T2>& p) const
		{
			const auto h1 = std::hash<T1>{}(p.first);
			const auto h2 = std::hash<T2>{}(p.second);

			return h1 ^ h2;
		}
	};
}

#endif