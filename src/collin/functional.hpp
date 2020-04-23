#pragma once

#include <functional>
#include <utility>
#include <tuple>

namespace collin
{
	namespace functional
	{
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

		struct tuple_hash
		{
			template<class... Args>
			std::size_t operator()(const std::tuple<Args...>& tup) const
			{
				std::size_t hash = 0;
				for_each(tup, [&](const auto& item) {
					hash ^= std::hash<decltype(item)>{}(item);
				});

				return hash;
			}
		};
	}
}