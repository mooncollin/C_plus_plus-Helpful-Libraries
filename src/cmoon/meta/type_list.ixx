export module cmoon.meta.type_list;

import <tuple>;
import <cstddef>;
import <concepts>;

import cmoon.tuple;

namespace cmoon::meta
{
	export
	template<class... Types>
	struct type_list
	{
		public:
			using index_type = std::size_t;
			using types = std::tuple<Types...>;

			static constexpr index_type size {std::tuple_size_v<types>};

			template<std::size_t N>
			using get_type = std::tuple_element_t<N, types>;

			constexpr type_list() noexcept = default;
			constexpr type_list(const std::tuple<Types...>&) noexcept {}

		private:
			template<class T, std::size_t I>
				requires (I != size)
			[[nodiscard]] static constexpr index_type get_index_helper() noexcept
			{
				if constexpr (std::same_as<T, get_type<I>>)
				{
					return I;
				}
				else
				{
					return get_index_helper<T, I + 1>();
				}
			}

			template<class T, std::size_t I>
			[[nodiscard]] static constexpr bool contains_helper() noexcept
			{
				if constexpr (I == size)
				{
					return false;
				}
				else if constexpr (std::same_as<T, get_type<I>>)
				{
					return true;
				}
				else
				{
					return contains_helper<T, I + 1>();
				}
			}
		public:
			template<class T>
			[[nodiscard]] static constexpr index_type get_index() noexcept
			{
				return get_index_helper<T, 0>();
			}

			template<class T>
			[[nodiscard]] static constexpr bool contains() noexcept
			{
				return contains_helper<T, 0>();
			}
	};

	export
	struct extract_tuple_types_t {};

	export
	inline constexpr extract_tuple_types_t extract_tuple_types {};

	template<class... Types>
	[[nodiscard]] type_list<Types...> make_type_list() noexcept
	{
		return type_list<Types...>{};
	}

	template<class... Types>
	[[nodiscard]] type_list<Types...> make_type_list(const std::tuple<Types...>&) noexcept
	{
		return type_list<Types...>{};
	}

	template<cmoon::tuple::tuple_type Tuple>
	[[nodiscard]] auto make_type_list(const extract_tuple_types_t) noexcept
	{
		return decltype(make_type_list(std::declval<Tuple>())){};
	}
}