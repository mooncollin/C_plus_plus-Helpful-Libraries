#pragma once

#include <utility>
#include <tuple>

namespace cmoon
{
    struct pair_hash
    {
        template<class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const
        {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    struct tuple_hash
    {
        template<class... Args>
        std::size_t operator()(const std::tuple<Args...>& tup) const
        {
            std::size_t hash = 0;
            for_each(tup, [&](const auto& item) {
                hash ^= std::hash<std::decay_t<decltype(item)>>{}(item);
                });

            return hash;
        }
    };
}