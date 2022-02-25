export module cmoon.meta.type_list;

import <tuple>;
import <cstddef>;
import <concepts>;
import <utility>;
import <functional>;
import <type_traits>;

import cmoon.concepts;
import cmoon.type_traits;

namespace cmoon::meta
{
	export
	template<typename... Types>
	struct type_list
	{
		template<typename... Types2>
		friend struct type_list;

		public:
			using index_type = std::size_t;
			using tuple_t = std::tuple<Types...>;

			[[nodiscard]] static constexpr index_type size() noexcept
			{
				return sizeof...(Types);
			}

			template<index_type N>
				requires(N < size())
			using type = std::tuple_element_t<N, tuple_t>;
		private:
			template<typename T, index_type I>
				requires (I != size())
			[[nodiscard]] static constexpr index_type index_helper() noexcept
			{
				if constexpr (std::same_as<T, type<I>>)
				{
					return I;
				}
				else
				{
					return index_helper<T, I + 1>();
				}
			}
		public:
			template<typename T>
			[[nodiscard]] static constexpr index_type index() noexcept
			{
				return index_helper<T, 0>();
			}

			template<typename T>
			[[nodiscard]] static constexpr bool contains() noexcept
			{
				return (std::same_as<T, Types> || ...);
			}

			[[nodiscard]] static constexpr bool is_unique() noexcept
			{
				return cmoon::is_unique_v<Types...>;
			}

			[[nodiscard]] static constexpr bool is_all_same_type() noexcept
			{
				if constexpr (size() == 0)
				{
					return true;
				}
				else
				{
					return cmoon::is_all_of_v<Types...>;
				}
			}

			template<template<typename> class Predicate>
			[[nodiscard]] static constexpr bool all_of() noexcept
			{
				return (Predicate<Types>::value && ...);
			}

			template<template<typename> class Predicate>
			[[nodiscard]] static constexpr bool any_of() noexcept
			{
				return (Predicate<Types>::value || ...);
			}

			template<template<typename> class Predicate>
			[[nodiscard]] static constexpr bool none_of() noexcept
			{
				return (!Predicate<Types>::value && ...);
			}

			template<template<typename> typename Function>
			static constexpr void for_each() noexcept(std::conjunction_v<std::is_nothrow_invocable<Function<Types>>...>)
			{
				(std::invoke(Function<Types>{}), ...);
			}

			template<typename... Types2>
			using append = type_list<Types..., Types2...>;
		private:
			template<typename...>
			struct concatenate_helper : std::type_identity<type_list> {};

			template<typename... Types2, template<typename...> typename TypeList1, typename... TypeListN>
			struct concatenate_helper<TypeList1<Types2...>, TypeListN...> : std::type_identity<
				typename type_list::template append<Types2...>::concatenate_helper<TypeListN...>::type
			> {};
		public:
			template<class... TypeLists>
			using concatenate = typename concatenate_helper<TypeLists...>::type;
		private:
			template<template<typename> class Predicate, typename...>
			struct filter_helper : std::type_identity<type_list<>> {};

			template<template<typename> class Predicate, typename Type1, typename... TypeN>
			struct filter_helper<Predicate, Type1, TypeN...> : std::type_identity<
				std::conditional_t<Predicate<Type1>::value,
					typename type_list<Type1>::template concatenate<typename filter_helper<Predicate, TypeN...>::type>,
					typename filter_helper<Predicate, TypeN...>::type
				>
			> {};

			template<index_type Offset, index_type Count>
			struct sub_list_helper : std::type_identity<
				typename type_list<type<Offset>>::template concatenate<
					typename sub_list_helper<Offset + 1, Count - 1>::type
				>
			> {};

			template<index_type Offset, index_type Count>
				requires(Offset >= size())
			struct sub_list_helper<Offset, Count> : std::type_identity<type_list<>> {};

			template<index_type Count>
			struct sub_list_helper<Count, 0> : std::type_identity<type_list<>> {};

			template<class Out, class In>
			struct unique_helper;

			template<class... Out, class In1, class... InN>
			struct unique_helper<type_list<Out...>, type_list<In1, InN...>>
			{
				using type = std::conditional_t<
					type_list<Out...>::template contains<In1>(),
					typename unique_helper<type_list<Out...>, type_list<InN...>>::type,
					typename unique_helper<type_list<Out..., In1>, type_list<InN...>>::type
				>;
			};

			template<class Out>
			struct unique_helper<Out, type_list<>>
			{
				using type = Out;
			};

			template<template<typename> class Predicate, class...>
			struct find_helper;

			template<template<typename> class Predicate, class T1, class... TN>
			struct find_helper<Predicate, T1, TN...> : std::type_identity<
				std::conditional_t<Predicate<T1>::value,
				T1,
				typename find_helper<Predicate, TN...>::type>
			> {};

		public:
			template<template<typename> class Predicate>
			using filter = typename filter_helper<Predicate, Types...>::type;

			template<template<typename> class Function>
			using transform = type_list<typename Function<Types>::type...>;

			template<index_type Offset, index_type Count = -1>
				requires(Offset <= size())
			using sub_list = typename sub_list_helper<Offset, Count>::type;

			using unique = typename unique_helper<type_list<>, type_list>::type;

			template<template<typename> class Predicate>
			[[nodiscard]] static constexpr bool can_find() noexcept
			{
				return size() > 0 && std::disjunction_v<Predicate<Types>::value...>;
			}

			template<template<typename> class Predicate>
				requires(can_find<Predicate>())
			using find = typename find_helper<Predicate, Types...>::type;

			template<template<typename...> typename T, typename... ExtraArgs>
			using complete_type = T<ExtraArgs..., Types...>;
	};

	export
	template<std::size_t Index, typename... Ts>
		requires(Index <= sizeof...(Ts))
	using get_type = typename type_list<Ts...>::template type<Index>;

	template<typename>
	struct extract_types_into_type_list_impl : std::type_identity<type_list<>> {};

	template<template<typename...> typename Tuple, typename... Types>
	struct extract_types_into_type_list_impl<Tuple<Types...>> : std::type_identity<type_list<Types...>> {};

	export
	template<class T>
	using extract_types_into_type_list = typename extract_types_into_type_list_impl<T>::type;

	export
	template<template<class> class Function, typename... Ts>
	using transform_types = typename type_list<Ts...>::template transform<Function>;

	export
	template<template<class> class Predicate, typename... Ts>
	using filter_types = typename type_list<Ts...>::template filter<Predicate>;

	export
	template<class... TypeLists>
	using concatenate_types = typename type_list<>::template concatenate<TypeLists...>;

	export
	template<typename... Ts>
	using unique_types = typename type_list<Ts...>::unique;
}
