#pragma once

#include <type_traits>
#include <algorithm>
#include <utility>

namespace collin
{
    namespace ranges
    {
        template<class T>
            requires std::is_move_constructible_v<T> &&
                     std::is_move_assignable_v<T> &&
                     std::is_destructible_v<T>
        void swap(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
        {
            std::swap(a, b);
        }

        template<class T, std::size_t N>
        void swap(T(&a)[N], T(&b)[N]) noexcept(noexcept(swap(*a, *b)))
        {
            std::swap_ranges(a, a + N, b);
        }

        template<class T, class U>
        void value_swap(T&& t, U&& u)
        {
            ranges::swap(std::forward<T>(t), std::forward<U>(u));
        }

        template<class T, class U>
        concept Same = std::is_same_v<T, U>;

        template<class T, class U>
        concept DerivedFrom = std::is_base_of_v<U, T>;

        template<class T, class U>
        concept ConvertibleTo = std::is_convertible_v<T, U>;

        template<class T, class U>
        concept Common = requires(T t)
        {
            typename std::common_type_t<T, U>;
            typename std::common_type_t<U, T>;
            Same<std::common_type_t<U, T>, std::common_type_t<T, U>>();
            std::common_type_t<T, U>(std::forward<T>(t));
            std::common_type_t<T, U>(std::forward<U>(u));
        };

        template<class T>
        concept Integral = std::is_integral_v<T>;

        template<class T>
        concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

        template<class T>
        concept UnsignedIntegral = Integral<T> && std::is_unsigned_v<T>;

        template<class T, class U>
        concept Assignable = requires(T&& t, U&& u)
        {
            {std::forward<T>(t) = std::forward<U>(u)} -> Same<T&>;
        };

        template<class T, class U = T>
        concept Swappable = requires(T && a, U && b)
        {
            Common<T, U>();
            ranges::swap(std::forward<T>(a), std::forward<T>(b));
            ranges::swap(std::forward<U>(b), std::forward<U>(b));
            ranges::swap(std::forward<U>(b), std::forward<T>(a));
            ranges::swap(std::forward<T>(a), std::forward<U>(b));
        };
    }
}