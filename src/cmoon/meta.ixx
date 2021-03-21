module;
#include <tuple>

export module cmoon.core:meta;

namespace cmoon
{
	namespace meta
	{
		template<class T>
		struct choice_t
		{
			T strategy {};
			bool no_throw {false};
		};

		template<class... Types>
		struct type_list
		{
			public:
			using index_type = std::size_t;
			using types = std::tuple<Types...>;

			static constexpr index_type size = std::tuple_size_v<types>;

			template<std::size_t N>
			using get_type = std::tuple_element_t<N, types>;

			private:
				template<class T, std::size_t I>
				[[nodiscard]] static constexpr index_type get_index_helper() noexcept
				{
					if constexpr (I == size)
					{
						static_assert(false, "Type does not exist in this type_list");
					}
					else if constexpr (std::same_as<T, get_type<I>>)
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
	}
}