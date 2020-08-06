#pragma once

#include <type_traits>
#include <cstddef>
#include <functional>
#include <iterator>
#include <tuple>

namespace collin
{
	namespace type_traits
	{
		template<class T, typename = void>
		struct is_type_complete : std::false_type {};

		template<class T>
		struct is_type_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

		template<class T>
		constexpr bool is_type_complete_v = is_type_complete<T>::value;

		template<class T, class S, class = void>
		struct is_stream_writable : std::false_type
		{
		};

		template<class T, class S>
		struct is_stream_writable<S, T, std::void_t<decltype(std::declval<S&>() << std::declval<const T&>())>> : std::true_type
		{
		};

		template<class T, class S>
		constexpr bool is_stream_writable_v = is_stream_writable<S, T>::value;

		template<class T, class = void>
		struct has_size : std::false_type
		{
		};

		template<class T>
		struct has_size<T, std::void_t<decltype(std::size(std::declval<T>()))>> : std::true_type
		{
		};

		template<class T, class = void>
		struct has_type : std::false_type {};

		template<class T>
		struct has_type<T, std::void_t<decltype(T::type)>> : std::true_type {};

		template<class T>
		constexpr auto has_type_v = has_type<T>::value;

		template<class T>
		constexpr bool has_size_v = has_size<T>::value;

		template<typename T>
		struct function_traits;

		template<typename R, typename... Args>
		struct function_traits<std::function<R(Args...)>>
		{
			static constexpr auto nargs = sizeof...(Args);
			using result_type = R;

			template<std::size_t i>
			struct arg
			{
				using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
			};
		};

		template<typename T, typename... Ts>
		struct contains : std::disjunction<std::is_same<T, Ts>...> {};

		template<typename T, typename... Ts>
		constexpr auto contains_v = contains<T, Ts...>::value;

		template<typename...>
		struct is_unique;

		template<>
		struct is_unique<> : std::true_type {};

		template<typename F, typename... T>
		struct is_unique<F, T...> : std::bool_constant<is_unique<T...>::value && !contains_v<F, T...>> {};

		template<typename... T>
		constexpr auto is_unique_v = is_unique<T...>::value;

		template<typename... T>
		struct common_type {};

		template<typename T>
		struct common_type<T> : std::decay_t<T> {};

		template<typename T, typename U>
		struct common_type<T, U> : std::conditional_t<(std::is_same_v<std::decay_t<T>, T> && std::is_same_v<std::decay_t<U>, U>), 
													   std::conditional_t<has_type_v<common_type<T, U>>,
																		  common_type<T, U>, 
																		  std::decay_t<decltype(false ? std::declval<const T&>() : std::declval<const U&>())>>,
													   common_type<std::decay_t<T>, std::decay_t<U>>> {};

		template<typename... T>
		using common_type_t = typename common_type<T...>::type;

		template<typename T, typename U, typename... R>
		struct common_type<T, U, R...> : common_type_t<common_type_t<T, U>, R...> {};

		template<class T, class U, template<class> class TQual, template<class> class UQual>
		struct basic_common_reference {};

		namespace details
		{
			template<class Function, typename... Args>
			using invoke = typename Function::template invoke<Args...>;

			template<typename>
			struct xref
			{
				template<typename T>
				using invoke = T;
			};

			template<typename T>
			struct xref<T&&>
			{
				template<typename U>
				using invoke = std::add_rvalue_reference_t<invoke<xref<T>, U>>;
			};

			template<typename T>
			struct xref<T&>
			{
				template<typename U>
					using invoke = std::add_lvalue_reference_t<invoke<xref<T>, U>>;
			};

			template<typename T>
			struct xref<T const>
			{
				template<typename U>
				using invoke = U const;
			};
		}

		template<typename... T>
		struct common_reference {};

		template<typename T>
		struct common_reference<T>
		{
			using type = T;
		};

		template<typename T, typename U>
		struct common_reference<T, U> : std::conditional_t<has_type_v<basic_common_reference<std::remove_cvref_t<T>, std::remove_cvref_t<U>, details::xref<T>::template invoke, details::xref<U>::template invoke>>,
														   basic_common_reference<std::remove_cvref_t<T>, std::remove_cvref_t<U>, details::xref<T>::template invoke, details::xref<U>::template invoke>,
														   common_type<T, U>> {};

		template<typename... T>
		using common_reference_t = typename common_reference<T...>::type;

		template<typename T, typename U, typename... R>
		struct common_reference<T, U, R...> : common_reference<common_reference_t<T, U>, R...> {};

		template<bool B, class T, class F, T t, F f>
		struct conditional_value;

		template<class T, class F, T t, F f>
		struct conditional_value<true, T, F, t, f>
		{
			static constexpr auto value = t;
		};

		template<class T, class F, T t, F f>
		struct conditional_value<false, T, F, t, f>
		{
			static constexpr auto value = f;
		};

		template<class T, T t, T t2>
		struct conditional_value<true, T, T, t, t2>
		{
			static constexpr auto value = t;
		};

		template<class T, T t, T t2>
		struct conditional_value<false, T, T, t, t2>
		{
			static constexpr auto value = t2;
		};

		template<bool B, class T, class F, T t, F f>
		constexpr auto conditional_value_v = conditional_value<B, T, F, t, f>::value;

		template<bool B, class T, T t, T t2>
		constexpr auto conditional_value2_v = conditional_value<B, T, T, t, t2>::value;

		template<class T>
		constexpr auto make_unsigned(T&& t) noexcept
		{
			return static_cast<std::make_unsigned_t<decltype(std::forward<T>(t))>>(std::forward<T>(t));
		}
	}
}