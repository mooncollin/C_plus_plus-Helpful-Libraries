export module cmoon.meta.value_list;

import <cstddef>;
import <type_traits>;
import <iterator>;
import <concepts>;
import <tuple>;
import <functional>;

import cmoon.concepts;

import cmoon.meta.type_list;

namespace cmoon::meta
{
	export
	template<auto... Values>
	struct value_list
	{
		template<auto... Values2>
		friend struct value_list;

		public:
			using index_type = std::size_t;
			using types = type_list<std::remove_cvref_t<decltype(Values)>...>;

			template<index_type N>
			using type = typename types::template type<N>;

			[[nodiscard]] static constexpr index_type size() noexcept
			{
				return sizeof...(Values);
			}
		private:
			template<index_type N, auto Value1, auto... ValueN>
			[[nodiscard]] static constexpr auto get_helper() noexcept
			{
				if constexpr (N == 0)
				{
					return Value1;
				}
				else
				{
					return get_helper<N - 1, ValueN...>();
				}
			}

			template<class T>
			[[nodiscard]] static constexpr bool contains_helper(const T&) noexcept
			{
				return false;
			}

			template<auto Value1, auto... ValueN, class T>
			[[nodiscard]] static constexpr bool contains_helper(const T& item) noexcept
			{
				if constexpr (std::same_as<std::remove_cvref_t<decltype(Value1)>,
										   std::remove_cvref_t<T>>)
				{
					if (Value1 == item)
					{
						return true;
					}
					else
					{
						return contains_helper<ValueN...>(item);
					}
				}
				else
				{
					return contains_helper<ValueN...>(item);
				}
			}
		public:
			template<index_type N>
				requires(N < size())
			[[nodiscard]] static constexpr auto get() noexcept
			{
				return get_helper<N, Values...>();
			}

			template<class T>
			[[nodiscard]] static constexpr bool contains(const T& item) noexcept
			{
				return contains_helper<Values...>(item);
			}

			[[nodiscard]] static constexpr auto to_tuple() noexcept
			{
				return std::make_tuple(Values...);
			}

			template<class F>
			[[nodiscard]] static constexpr bool all_of(F&& func) noexcept
			{
				return (func(Values) && ...);
			}

			template<class F>
			[[nodiscard]] static constexpr bool any_of(F&& func) noexcept
			{
				return (func(Values) || ...);
			}

			template<class F>
			[[nodiscard]] static constexpr bool none_of(F&& func) noexcept
			{
				return (!func(Values) && ...);
			}

			template<class F>
			static constexpr void for_each(F&& func) noexcept
			{
				(func(Values), ...);
			}

			template<auto... Values2>
			using append = value_list<Values..., Values2...>;
		private:
			template<typename...>
			struct concatenate_helper : std::type_identity<value_list> {};

			template<auto... Types2, template<auto...> typename ValueList1, typename... ValueListN>
			struct concatenate_helper<ValueList1<Types2...>, ValueListN...> : std::type_identity<
				typename value_list::template append<Types2...>::concatenate_helper<ValueListN...>::type
			> {};
		public:
			template<typename... ValueList>
			using concatenate = typename concatenate_helper<ValueList...>::type;
		private:
			template<template<auto> class Predicate, auto...>
			struct filter_helper : std::type_identity<value_list<>> {};

			template<template<auto> class Predicate, auto Value1, auto... ValueN>
			struct filter_helper<Predicate, Value1, ValueN...> : std::type_identity<
				std::conditional_t<Predicate<Value1>::value,
					typename value_list<Value1>::template concatenate<typename filter_helper<Predicate, ValueN...>::type>,
					typename filter_helper<Predicate, ValueN...>::type
				>
			> {};

			template<index_type Offset, index_type Count>
			struct sub_list_helper : std::type_identity<
				typename value_list<get<Offset>()>::template concatenate<
					typename sub_list_helper<Offset + 1, Count - 1>::type
				>
			> {};

			template<index_type Offset, index_type Count>
				requires(Offset >= size())
			struct sub_list_helper<Offset, Count> : std::type_identity<value_list<>> {};

			template<index_type Count>
			struct sub_list_helper<Count, 0> : std::type_identity<value_list<>> {};

			template<template<auto> class Function, auto... Values2>
			using transform_to_types_helper = type_list<typename Function<Values2>::type...>;
		public:
			template<template<auto> class Predicate>
			using filter = typename filter_helper<Predicate, Values...>::type;

			template<template<auto> class Function>
			using transform = value_list<Function<Values>::value...>;

			// helper needed because MSVC cannot do this properly:
			// type_list<typename Function<Values>::type...>
			// So when I delay it to a helper, MSVC is fine with that.
			template<template<auto> class Function>
			using transform_to_types = transform_to_types_helper<Function, Values...>;

			template<index_type Offset, index_type Count = -1>
				requires(Offset < size())
			using sub_list = typename sub_list_helper<Offset, Count>::type;
	};

	export
	template<template<auto> class Function, auto... Values>
	using transform_values = typename value_list<Values...>::template transform<Function>;

	export
	template<template<auto> class Predicate, auto... Values>
	using filter_values = typename value_list<Values...>::template filter<Predicate>;

	export
	template<typename... ValueList>
	using concatenate_values = typename value_list<>::template concatenate<ValueList...>;

	template<auto CurrentValue, auto Finish, class Operation>
	struct iota_impl : std::type_identity<
		concatenate_values<value_list<CurrentValue>,
						   typename iota_impl<Operation{}(CurrentValue, 1), Finish, Operation>::type>
	> {};

	template<auto CurrentValue, auto Finish, class Operation>
		requires(CurrentValue == Finish)
	struct iota_impl<CurrentValue, Finish, Operation> : std::type_identity<value_list<>> {};

	export
	template<auto Start, auto Finish>
		requires(std::integral<decltype(Start)> &&
				 std::integral<decltype(Finish)> &&
				 std::same_as<decltype(Start), decltype(Finish)>)
	using iota = typename iota_impl<Start, Finish, std::conditional_t<Start <= Finish, std::plus<>, std::minus<>>>::type;

	export
	template<std::size_t... Ints>
	using index_sequence = value_list<Ints...>;

	export
	template<class T, T N>
	using make_integer_sequence = iota<static_cast<T>(0), N>;

	export
	template<std::size_t N>
	using make_index_sequence = make_integer_sequence<std::size_t, N>;

	export
	template<class... T>
	using index_sequence_for = make_index_sequence<sizeof...(T)>;
}

namespace std
{
	export
	template<auto... Values>
	[[nodiscard]] constexpr typename cmoon::meta::value_list<Values...>::index_type size(const cmoon::meta::value_list<Values...>) noexcept
	{
		return cmoon::meta::value_list<Values...>::size();
	}

	export
	template<typename cmoon::meta::value_list<>::index_type I, auto... Values>
	[[nodiscard]] constexpr auto get(const cmoon::meta::value_list<Values...>) noexcept
	{
		return cmoon::meta::value_list<Values...>::template get<I>();
	}
}