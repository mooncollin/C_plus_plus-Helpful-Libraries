export module cmoon.iterator.enumerate;

import <iterator>;
import <cstdint>;
import <utility>;

namespace cmoon
{
	export
	template<std::input_iterator InputIterator>
	class enumerate
	{
		public:
			using difference_type = std::intmax_t;
			using value_type = std::pair<std::intmax_t, typename std::iterator_traits<InputIterator>::value_type>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::input_iterator_tag;

			constexpr explicit enumerate(InputIterator begin, InputIterator end, difference_type start = 0, difference_type step = 1)
				: current_{begin}, end_{end}, index_{start}, step_{step} {}

			constexpr enumerate begin()
			{
				return *this;
			}

			constexpr enumerate end() const
			{
				return enumerate(end_, end_);
			}

			constexpr enumerate& operator++()
			{
				++current_;
				index_ += step_;

				return *this;
			}

			constexpr enumerate operator++(int)
			{
				auto e = *this;
				operator++();
				return e;
			}

			constexpr auto operator*() const
			{
				return std::make_pair(index_, current_);
			}

			constexpr bool operator==(const enumerate& rhs) const
			{
				return current_ == rhs.current_;
			}

			constexpr bool operator!=(const enumerate& rhs) const
			{
				return !operator==(rhs);
			}

			constexpr difference_type index() const noexcept
			{
				return index_;
			}

			constexpr difference_type step() const noexcept
			{
				return step_;
			}

		private:
			InputIterator current_;
			InputIterator end_;
			difference_type index_;
			difference_type step_;
	};
}