export module cmoon.concepts;

import <type_traits>;
import <concepts>;
import <cstddef>;
import <iterator>;
import <memory>;
import <format>;
import <string>;

import cmoon.type_traits;

namespace cmoon
{
	export
	template<class T>
	concept object = std::is_object_v<T>;

	export
	template<class T>
	concept array = std::is_array_v<T>;

	export
	template<class F, class Result>
	concept supplier = !std::is_void_v<Result> &&
						std::is_invocable_r_v<Result, F>;
	
	export
	template<class F, class Result>
	concept unary_operator = std::is_invocable_r_v<Result, F, Result>;

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
	concept signed_arithmetic = std::is_signed_v<T>;

	export
	template<class T>
	concept unsigned_arithmetic = std::is_unsigned_v<T>;

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
	template<class T>
	concept string_literal = cmoon::is_string_literal_v<T>;

	export
	template<class T>
	concept character = cmoon::is_character_v<T>;

	export
	template<class T, class CharT = char, class Out = std::back_insert_iterator<std::basic_string<CharT>>>
	concept formattable = 
		requires(const T& t, std::basic_format_context<Out, CharT>& ctx, std::formatter<T, CharT> f)
	{
		f.format(t, ctx);
	};

	export
	template<class Type, template<typename...> class Template>
	concept specialization_of = cmoon::is_specialization_v<Type, Template>;

	export
	template<class T>
	concept movable_value =  std::move_constructible<std::decay_t<T>> &&
							 std::constructible_from<std::decay_t<T>, T>;

	export
	template<class From, class To>
	concept decays_to = std::same_as<std::decay_t<From>, To>;

	export
	template<class T>
	concept class_type = decays_to<T, T> && std::is_class_v<T>;
}