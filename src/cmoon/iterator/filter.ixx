export module cmoon.iterator.filter;

import <iterator>;
import <concepts>;
import <functional>;
import <utility>;

namespace cmoon
{
	template<std::input_iterator InputIt, std::invocable<std::iter_value_t<InputIt>> Function>
	class filter_impl
	{
		public:
			using difference_type = std::iter_difference_t<InputIt>;
			using value_type = std::iter_value_t<InputIt>;
			using pointer = typename std::iterator_traits<InputIt>::pointer;
			using reference = std::iter_reference_t<InputIt>;
			using iterator_category = std::input_iterator_tag;

			filter_impl(InputIt begin, InputIt end, Function& f)
				: current_{begin}, end_{end}, function_{f}
			{
				operator++();
			}

			reference operator*()
			{
				return *current_;
			}

			pointer operator->()
			{
				return std::addressof(*current_);
			}

			filter_impl& operator++()
			{
				while (current_ != end_ && !function_.get()(*current_))
				{
					++current_;
				}

				return *this;
			}

			filter_impl operator++(int)
			{
				auto r = *this;
				++*this;
				return r;
			}
					
			[[nodiscard]] friend constexpr bool operator==(const filter_impl& lhs, const filter_impl& rhs) noexcept
			{
				return lhs.current_ == rhs.current_;
			}

			[[nodiscard]] friend constexpr bool operator!=(const filter_impl& lhs, const filter_impl& rhs) noexcept
			{
				return !(lhs == rhs);
			}
		private:
			InputIt current_;
			InputIt end_;
			std::reference_wrapper<Function> function_;
	};

	export
	template<std::input_iterator InputIt, std::invocable<std::iter_value_t<InputIt>> Function>
	class filter
	{
		public:
			using difference_type = std::iter_difference_t<InputIt>;
			using value_type = std::iter_value_t<InputIt>;
			using pointer = typename std::iterator_traits<InputIt>::pointer;
			using reference = std::iter_reference_t<InputIt>;
			using iterator = details::filter_impl<InputIt, Function>;

			constexpr filter(InputIt begin, InputIt end, Function&& f)
				: begin_{begin}, end_{end}, function_{std::forward<Function>(f)} {}

			constexpr iterator begin() const noexcept
			{
				return iterator{begin_, end_, function_};
			}

			constexpr iterator end() const noexcept
			{
				return iterator{end_, end_, function_};
			}
		private:
			InputIt begin_;
			InputIt end_;
			Function function_;
	};
}