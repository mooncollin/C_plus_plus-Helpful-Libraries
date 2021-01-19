#pragma once

#include <type_traits>
#include <concepts>

#include "cmoon/type_traits.hpp"

namespace cmoon
{
	template<class T>
	concept object = std::is_object_v<T>;

	template<class T>
	concept array = std::is_array_v<T>;

	template<class F, class Result, class... Args>
	concept supplier = std::invocable<F, Args...> &&
					   std::same_as<std::invoke_result_t<F, Args...>, Result>;

	template<class F, class Result>
	concept unary_operator = supplier<F, Result, Result>;

	template<class F, class... Args>
	concept consumer = std::invocable<F, Args...> &&
					   std::is_void_v<std::invoke_result_t<F, Args...>>;

	template<class F, class... Args>
	concept function_object = object<F> && std::invocable<F, Args...>;

	template<class T>
	concept arithmetic = std::is_arithmetic_v<T>;

	template<class T>
	concept complete_type = 
		requires
	{
		sizeof(T);
	};

	template<class T>
	concept has_size = 
		requires(T t)
	{
		std::size(t);
	};

	template<class T>
	concept has_type = 
		requires
	{
		typename T::type;
	};

	template<class T, class S>
	concept stream_writable =
		requires(const T& t, S& s)
	{
		{ s << t };
	};

	template<class A>
	concept allocator = std::copyable<A> &&
		requires
	{
		typename std::allocator_traits<A>;
		typename A::value_type;
	} &&
		requires(A a, typename std::allocator_traits<A>::size_type n, typename std::allocator_traits<A>::pointer p)
	{
		{ a.allocate(n) } -> std::same_as<typename std::allocator_traits<A>::pointer>;
		{ a.deallocate(p, n) };
		{ a == a } -> std::convertible_to<bool>;
		{ a != a } -> std::convertible_to<bool>;
	};

	template<class T>
	concept string_literal = cmoon::is_string_literal_v<T>;

	template<class T>
	concept char_literal = cmoon::is_char_literal_v<T>;
}