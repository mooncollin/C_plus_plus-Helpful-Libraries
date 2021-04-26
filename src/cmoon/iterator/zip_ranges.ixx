export module cmoon.iterator.zip_ranges;

import <ranges>;
import <functional>;
import <cstddef>;
import <iterator>;

namespace cmoon
{
	export
	template<std::ranges::range... Ranges>
		requires(sizeof...(Ranges) > 0)
	class zip_ranges
	{
		using iterators_type = std::tuple<std::ranges::iterator_t<Ranges>...>;
		using ranges_type = std::tuple<std::reference_wrapper<Ranges>...>;
		static constexpr auto range_size = sizeof...(Ranges);

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::tuple<std::ranges::range_value_t<Ranges>...>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::input_iterator_tag;

			constexpr zip_ranges(Ranges&... ranges)
				: ranges_{std::make_tuple(std::reference_wrapper<Ranges>{ranges}...)},
				  current_{std::make_tuple(std::begin(ranges)...)} {}

			constexpr zip_ranges& operator++()
			{
				cmoon::tuples::for_each(current_, [](auto& v) { ++v; });
				return *this;
			}

			constexpr zip_ranges operator++(int)
			{
				auto r = *this;
				operator++();
				return r;
			}

			constexpr value_type operator*() const
			{
				return dereference_helper(std::make_index_sequence<range_size>{});
			}

			constexpr value_type operator*()
			{
				return dereference_helper(std::make_index_sequence<range_size>{});
			}

			constexpr zip_ranges begin()
			{
				return *this;
			}

			constexpr zip_ranges end() const
			{
				return end_helper(std::make_index_sequence<range_size>{});
			}

			[[nodiscard]] constexpr bool operator==(const zip_ranges& rhs) const noexcept
			{
				return equal_helper(rhs, std::make_index_sequence<range_size>{});
			}

			[[nodiscard]] constexpr bool operator!=(const zip_ranges& rhs) const noexcept
			{
				return !(*this == rhs);
			}
		private:
			ranges_type ranges_;
			iterators_type current_;

			struct do_end {};

			constexpr zip_ranges(do_end, Ranges&... ranges)
				: ranges_{std::make_tuple(std::reference_wrapper<Ranges>{ranges}...)},
					current_{std::make_tuple(std::end(ranges)...)} {}

			template<std::size_t... I>
			[[nodiscard]] constexpr zip_ranges end_helper(std::index_sequence<I...>) const noexcept
			{
				return {do_end{}, std::get<I>(ranges_).get()...};
			}

			template<std::size_t... I>
			constexpr value_type dereference_helper(std::index_sequence<I...>)
			{
				return std::make_tuple((*std::get<I>(current_))...);
			}

			template<std::size_t... I>
			constexpr value_type dereference_helper(std::index_sequence<I...>) const
			{
				return std::make_tuple((*std::get<I>(current_))...);
			}

			template<std::size_t... I>
			[[nodiscard]] constexpr bool equal_helper(const zip_ranges& rhs, std::index_sequence<I...>) const noexcept
			{
				return ((std::get<I>(current_) == std::get<I>(rhs.current_)) || ...);
			}
	};
}