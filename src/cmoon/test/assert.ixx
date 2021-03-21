module;
#include <iostream>
#include <sstream>
#include <string_view>
#include <algorithm>
#include <concepts>

export module cmoon.test.assert;
import cmoon.concepts;
import cmoon.format;
import cmoon.test.assert_exception;

namespace cmoon::test
{
	template<cmoon::can_format<> T, cmoon::can_format<> F>
	[[nodiscard]] inline std::string output_if_possible(const T& first, const F& second)
	{
		return cmoon::format("First:\n{}\nSecond:\n{}\n", first, second);
	}

	template<class T, class F>
	[[nodiscard]] inline std::string output_if_possible(const T&, const F&)
	{
		return "";
	}

	export
	template<class T, class F>
		requires(std::equality_comparable_with<T, F>)
	void assert_equal(const T& first, const F& second, std::string_view message="")
	{
		if(first != second)
		{
			throw assert_exception{cmoon::format("assert_equal failed: {}\n{}", message, output_if_possible(first, second))};
		}
	}

	export
	template<class T, class F>
		requires(std::equality_comparable_with<T, F>)
	void assert_not_equal(const T& first, const F& second, std::string_view message = "")
	{
		if (first == second)
		{
			throw assert_exception{cmoon::format("assert_not_equal failed: {}\n{}", message, output_if_possible(first, second))};
		}
	}

	export
	template<std::convertible_to<bool> T>
	void assert_true(const T& first, std::string_view message="")
	{
		if (!static_cast<bool>(first))
		{
			throw assert_exception{cmoon::format("assert_true failed: {}\n", message)};
		}
	}

	export
	template<std::convertible_to<bool> T>
	void assert_false(const T& first, std::string_view message = "")
	{
		if (static_cast<bool>(first))
		{
			throw assert_exception{cmoon::format("assert_false failed: {}\n", message)};
		}
	}

	export
	template<class T, class F>
		requires(std::equality_comparable_with<T*, F*>)
	void assert_is(const T& first, const F& second, std::string_view message = "")
	{
		if (std::addressof(first) != std::addressof(second))
		{
			throw assert_exception{cmoon::format("assert_is failed: {}\n{}", message, output_if_possible(std::addressof(first), std::addressof(second)))};
		}
	}

	export
	template<class T, class F>
		requires(std::equality_comparable_with<T*, F*>)
	void assert_is_not(const T& first, const F& second, std::string_view message = "")
	{
		if (std::addressof(first) == std::addressof(second))
		{
			throw assert_exception{cmoon::format("assert_is_not failed: {}\n{}", message, output_if_possible(std::addressof(first), std::addressof(second)))};
		}
	}

	export
	template<class T, class F>
	void assert_is_instance(const T& first, std::string_view message = "")
	{
		const auto first_type = typeid(first);
		const auto second_type = typeid(F);
		if (first_type != second_type)
		{
			throw assert_exception{cmoon::format("assert_is_instance failed: {}\n{}", message, output_if_possible(first_type.name(), second_type.name()))};
		}
	}

	export
	template<class T, class F>
	void assert_is_not_instance(const T& first, std::string_view message = "")
	{
		const auto first_type = typeid(first);
		const auto second_type = typeid(F);
		if (first_type == second_type)
		{
			throw assert_exception{cmoon::format("assert_is_not_instance failed: {}\n{}", message, output_if_possible(first_type.name(), second_type.name()))};
		}
	}

	export
	template<class T, class Container>
	void assert_in(const T& member, const Container& container, std::string_view message = "")
	{
		if (std::ranges::find(container, member) == std::end(container))
		{
			throw assert_exception{cmoon::format("assert_in failed: {}\n", message)};
		}
	}

	export
	template<class T, class Container>
	void assert_not_in(const T& member, const Container& container, std::string_view message = "")
	{
		if (std::ranges::find(container, member) != std::end(container))
		{
			throw assert_exception{cmoon::format("assert_not_in failed: {}\n", message)};
		}
	}

	export
	template<class T, class F, class... Args>
		requires(std::invocable<F, Args...>)
	void assert_throws(const T& exception, F&& callable, std::string_view message, Args&&... args)
	{
		try
		{
			std::forward<F>(callable)(std::forward<Args>(args)...);
			throw assert_exception{cmoon::format("assert_throws failed: {}\n", message)};
		}
		catch (decltype(exception))
		{
			// Do nothing, it passed.
		}
	}

	export
	template<class T, class F, class... Args>
		requires(std::invocable<F, Args...>)
	void assert_throws(const T& exception, F&& callable, Args&&... args)
	{
		assert_throws(exception, callable, std::string_view{""}, std::forward<Args>(args)...);
	}

	export
	template<cmoon::arithmetic T, cmoon::arithmetic F>
	void assert_almost_equal(const T& first, const F& second, const std::common_type_t<T, F>& delta, std::string_view message = "")
	{
		const auto difference = std::abs(first - second);
		if (difference > delta)
		{
			auto full_message = cmoon::format("assert_almost_equal failed: {}\n{}", message, output_if_possible(first, second));
			if constexpr (cmoon::can_format<decltype(difference)>)
			{
				full_message += cmoon::format("Difference: {}\n", difference);
			}
			throw assert_exception{full_message};
		}
	}

	export
	template<cmoon::arithmetic T, cmoon::arithmetic F>
	void assert_not_almost_equal(const T& first, const F& second, const std::common_type_t<T, F>& delta, std::string_view message = "")
	{
		const auto difference = std::abs(first - second);
		if (difference <= delta)
		{
			auto full_message = cmoon::format("assert_almost_not_equal failed: {}\n{}", message, output_if_possible(first, second));
			if constexpr (cmoon::can_format<decltype(difference)>)
			{
				full_message += cmoon::format("Difference: {}\n", difference);
			}
			throw assert_exception{full_message};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_greater(const T& first, const F& second, std::string_view message = "")
	{
		if (!(first > second))
		{
			throw assert_exception{cmoon::format("assert_greater failed: {}\n{}", message, output_if_possible(first, second))};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_greater_equal(const T& first, const F& second, std::string_view message = "")
	{
		if (!(first >= second)) // Can't know for sure if first < second is supported
		{
			throw assert_exception{cmoon::format("assert_greater_equal failed: {}\n{}", message, output_if_possible(first, second))};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_less(const T& first, const F& second, std::string_view message = "")
	{
		if (!(first < second)) // Can't know for sure if first >= second is supported
		{
			throw assert_exception{cmoon::format("assert_less failed: {}\n{}", message, output_if_possible(first, second))};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_less_equal(const T& first, const F& second, std::string_view message = "")
	{
		if (!(first <= second)) // Can't know for sure if first > second is supported
		{
			throw assert_exception{cmoon::format("assert_less_equal failed: {}\n{}", message, output_if_possible(first, second))};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_equal(InputIterator1 first1, InputIterator1 end1, InputIterator2 first2, std::string_view message = "")
	{
		if (!std::equal(first1, end1, first2))
		{
			throw assert_exception{cmoon::format("assert_sequence_equal failed: {}\n", message)};
		}
	}

	export
	template<std::ranges::input_range Range1, std::ranges::input_range Range2,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											std::equal_to<>,
											Proj1,
											Proj2>)
	void assert_sequence_equal(Range1&& r1, Range2&& r2, std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {})
	{
		if (!std::ranges::equal(r1, r2, std::ranges::equal_to, proj1, proj2))
		{
			throw assert_exception{cmoon::format("assert_sequence_equal failed: {}\n", message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_almost_equal(InputIterator1 first1, InputIterator1 end1, InputIterator2 first2, 
		decltype(std::abs(std::declval<std::iter_value_t<InputIterator1>>() - std::declval<std::iter_value_t<InputIterator2>>())) delta,
		std::string_view message = "")
	{
		for(; first1 != end1; ++first1, ++first2)
		{
			const auto first = *first1;
			const auto second = *first2;
			const auto difference = std::abs(first - second);
			if (difference > delta)
			{
				throw assert_exception{cmoon::format("assert_sequence_almost_equal failed: {}\n{}", message, output_if_possible(first, second))};
			}
		}
	}

	export
	template<std::ranges::input_range Range1, std::ranges::input_range Range2,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											std::equal_to<>,
											Proj1,
											Proj2>)
	void assert_sequence_almost_equal(Range1&& r1, Range2&& r2, 
		decltype(std::abs(std::declval<std::ranges::range_value_t<Range1>>() - std::declval<std::ranges::range_value_t<Range2>>())) delta,
		std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {})
	{
		if (std::ranges::equal(r1, r2, [delta](const auto& first, const auto& second) { 
			const auto difference = std::abs(first - second);
			return difference <= delta;
		}, proj1, proj2))
		{
			throw assert_exception{cmoon::format("assert_sequence_almost_equal failed: {}\n", message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_not_almost_equal(InputIterator1 first1, InputIterator1 end1, InputIterator2 first2, decltype(std::abs(*std::declval<InputIterator1>() - *std::declval<InputIterator2>())) delta, std::string_view message = "")
	{
		for(; first1 != end1; ++first1, ++first2)
		{
			const auto first = *first1;
			const auto second = *first2;
			const auto difference = std::abs(first - second);
			if (difference <= delta)
			{
				throw assert_exception{cmoon::format("assert_sequence_not_almost_equal failed: {}\n{}", message, output_if_possible(first, second))};
			}
		}
	}

	export
	template<std::ranges::input_range Range1, std::ranges::input_range Range2,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											std::equal_to<>,
											Proj1,
											Proj2>)
	void assert_sequence_not_almost_equal(Range1&& r1, Range2&& r2, 
		decltype(std::abs(std::declval<std::ranges::range_value_t<Range1>>() - std::declval<std::ranges::range_value_t<Range2>>())) delta,
		std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {})
	{
		if (std::ranges::equal(r1, r2, [delta](const auto& first, const auto& second) { 
			const auto difference = std::abs(first - second);
			return difference > delta;
		}, proj1, proj2))
		{
			throw assert_exception{cmoon::format("assert_sequence_not_almost_equal failed: {}\n", message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_not_equal(InputIterator1 first1, InputIterator1 end1, InputIterator2 first2, std::string_view message = "")
	{
		if (std::equal(first1, end1, first2))
		{
			throw assert_exception{cmoon::format("assert_sequence_not_equal failed: {}\n", message)};
		}
	}

	export
	template<std::ranges::input_range Range1, std::ranges::input_range Range2,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											std::equal_to<>,
											Proj1,
											Proj2>)
	void assert_sequence_not_equal(Range1&& r1, Range2&& r2, std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {})
	{
		if (std::ranges::equal(r1, r2, std::ranges::equal_to, proj1, proj2))
		{
			throw assert_exception{cmoon::format("assert_sequence_not_equal failed: {}\n", message)};
		}
	}
}