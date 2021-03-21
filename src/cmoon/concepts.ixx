module;
#include <type_traits>
#include <concepts>

#include "cmoon/type_traits.hpp"

export module cmoon.concepts;

namespace cmoon
{
	export
	template<class T>
	concept object = std::is_object_v<T>;

	export
	template<class T>
	concept array = std::is_array_v<T>;

	export
	template<class F, class Result, class... Args>
	concept supplier = std::invocable<F, Args...> &&
					   std::same_as<std::invoke_result_t<F, Args...>, Result>;
	
	export
	template<class F, class Result>
	concept unary_operator = supplier<F, Result, Result>;

	export
	template<class F, class... Args>
	concept consumer = std::invocable<F, Args...> &&
					   std::is_void_v<std::invoke_result_t<F, Args...>>;
	
	export
	template<class F, class... Args>
	concept function_object = object<F> && std::invocable<F, Args...>;

	export
	template<class T>
	concept arithmetic = std::is_arithmetic_v<T>;

	export
	template<class T>
	concept complete_type = 
		requires
	{
		sizeof(T);
	};

	export
	template<class T>
	concept has_size = 
		requires(T t)
	{
		std::size(t);
	};

	export
	template<class T>
	concept has_type = 
		requires
	{
		typename T::type;
	};

	export
	template<class T, class S>
	concept stream_writable =
		requires(const T& t, S& s)
	{
		{ s << t };
	};

	export
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

	export
	template<class T>
	concept string_literal = cmoon::is_string_literal_v<T>;

	export
	template<class T>
	concept char_literal = cmoon::is_char_literal_v<T>;

	export
	template<typename T, typename R>
	concept nothrow_comparable_with = std::equality_comparable_with<T, R> &&
		requires(T t, R r)
	{
		{ t == r } noexcept;
		{ r == t } noexcept;
		{ t != r } noexcept;
		{ r != t } noexcept;
	};
}