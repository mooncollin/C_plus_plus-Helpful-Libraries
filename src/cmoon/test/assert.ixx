export module cmoon.test.assert;

import <iostream>;
import <sstream>;
import <string_view>;
import <algorithm>;
import <concepts>;
import <format>;
import <type_traits>;
import <source_location>;
import <ranges>;

import cmoon.concepts;

import cmoon.test.assert_exception;

namespace cmoon::test
{
	template<class T, class F>
	[[nodiscard]] inline std::string output_if_possible(const T& actual, const F& expected)
	{
		if constexpr (cmoon::formattable<std::decay_t<T>> &&
				      cmoon::formattable<std::decay_t<F>>)
		{
			return std::format("Actual:\n{}\nExpected:\n{}\n", actual, expected);
		}
		else if constexpr (cmoon::stream_writable<std::decay_t<T>, std::stringstream> &&
						   cmoon::stream_writable<std::decay_t<F>, std::stringstream>)
		{
			std::stringstream ss;
			ss << "Actual:\n"
			   << actual
			   << "\nExpected:\n"
			   << expected
			   << "\n";

			return ss.str();
		}
		else
		{
			return "";
		}
	}

	export
	template<class T, class F>
		requires(requires(const T& t, const F& f) {
			{ t == f } -> std::convertible_to<bool>;
		})
	void assert_equal(const T& actual, const F& expected, std::string_view message="", const std::source_location& location = std::source_location::current())
	{
		if(!(actual == expected))
		{
			throw assert_exception{std::format("assert_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
		}
	}

	export
	template<class T, class F>
		requires(requires(const T& t, const F& f) {
			{ t != f } -> std::convertible_to<bool>;
		})
	void assert_not_equal(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (!(actual != expected))
		{
			throw assert_exception{std::format("assert_not_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
		}
	}

	export
	template<std::convertible_to<bool> T>
	void assert_true(const T& actual, std::string_view message="", const std::source_location& location = std::source_location::current())
	{
		if (!static_cast<bool>(actual))
		{
			throw assert_exception{std::format("assert_true failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<std::convertible_to<bool> T>
	void assert_false(const T& actual, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (static_cast<bool>(actual))
		{
			throw assert_exception{std::format("assert_false failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<class T, class F>
		requires(std::equality_comparable_with<T*, F*>)
	void assert_is(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (std::addressof(actual) != std::addressof(expected))
		{
			throw assert_exception{std::format("assert_is failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(std::addressof(actual), std::addressof(expected)))};
		}
	}

	export
	template<class T, class F>
		requires(std::equality_comparable_with<T*, F*>)
	void assert_is_not(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (std::addressof(actual) == std::addressof(expected))
		{
			throw assert_exception{std::format("assert_is_not failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(std::addressof(actual), std::addressof(expected)))};
		}
	}

	export
	template<class Actual, class Expected>
	void assert_is_type(std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if constexpr (!std::same_as<Actual, Expected>)
		{
			throw assert_exception{std::format("assert_is_type failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(typeid(Actual).name(), typeid(Expected).name()))};
		}
	}

	export
	template<class Actual, class Expected>
	void assert_is_not_type(std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if constexpr (std::same_as<Actual, Expected>)
		{
			throw assert_exception{std::format("assert_is_not_type failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(typeid(Actual).name(), typeid(Expected).name()))};
		}
	}

	export
	template<class Actual, template<class...> class Expected>
	void assert_is_specialization_of(std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if constexpr (!cmoon::specialization_of<Actual, Expected>)
		{
			throw assert_exception{std::format("assert_is_specialization_of failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(typeid(Actual).name(), typeid(Expected).name()))};
		}
	}

	export
	template<class Actual, template<class...> class Expected>
	void assert_is_not_specialization_of(std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if constexpr (cmoon::specialization_of<Actual, Expected>)
		{
			throw assert_exception{std::format("assert_is_not_specialization_of failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(typeid(Actual).name(), typeid(Expected).name()))};
		}
	}

	export
	template<class Expected, class Actual>
	void assert_is_instance(const Actual& actual, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		const auto casted = dynamic_cast<const Expected*>(std::addressof(actual));
		if (casted == nullptr)
		{
			throw assert_exception{std::format("assert_is_instance failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(typeid(Actual).name(), typeid(Expected).name()))};
		}
	}

	export
	template<class Expected, class Actual>
	void assert_is_not_instance(const Actual& actual, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		const auto casted = dynamic_cast<const Expected*>(std::addressof(actual));
		if (casted != nullptr)
		{
			throw assert_exception{std::format("assert_is_not_instance failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(typeid(Actual).name(), typeid(Expected).name()))};
		}
	}

	export
	template<std::ranges::input_range Range, class T>
	void assert_in(const T& member, const Range& r, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::find(r, member) == std::ranges::end(r))
		{
			throw assert_exception{std::format("assert_in failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<class T, class Container>
	void assert_not_in(const T& member, const Container& container, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::find(container, member) != std::end(container))
		{
			throw assert_exception{std::format("assert_not_in failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<class T, class F, class... Args>
		requires(std::invocable<F, Args...>)
	void assert_throws(const T& exception, F&& callable, std::string_view message, Args&&... args, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::forward<F>(callable)(std::forward<Args>(args)...);
			throw assert_exception{std::format("assert_throws failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
		catch (decltype(exception))
		{
			// Do nothing, it passed.
		}
	}

	export
	template<class T, class F, class... Args>
		requires(std::invocable<F, Args...>)
	void assert_throws(const T& exception, F&& callable, Args&&... args, const std::source_location& location = std::source_location::current())
	{
		assert_throws(exception, callable, std::string_view{""}, std::forward<Args>(args)..., location);
	}

	export
	template<cmoon::arithmetic T, cmoon::arithmetic F>
	void assert_almost_equal(const T& actual, const F& expected, const std::common_type_t<T, F>& delta, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		const auto difference = std::abs(actual - expected);
		if (difference > delta)
		{
			auto full_message = std::format("assert_almost_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected));
			if constexpr (cmoon::formattable<decltype(difference)>)
			{
				full_message += std::format("Difference: {}\n", difference);
			}
			throw assert_exception{full_message};
		}
	}

	export
	template<cmoon::arithmetic T, cmoon::arithmetic F>
	void assert_not_almost_equal(const T& actual, const F& expected, const std::common_type_t<T, F>& delta, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		const auto difference = std::abs(actual - expected);
		if (difference <= delta)
		{
			auto full_message = std::format("assert_almost_not_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected));
			if constexpr (cmoon::formattable<decltype(difference)>)
			{
				full_message += std::format("Difference: {}\n", difference);
			}
			throw assert_exception{full_message};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_greater(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (!(actual > expected))
		{
			throw assert_exception{std::format("assert_greater failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_greater_equal(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (!(actual >= expected))
		{
			throw assert_exception{std::format("assert_greater_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_less(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (!(actual < expected))
		{
			throw assert_exception{std::format("assert_less failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
		}
	}

	export
	template<class T, class F>
		requires(std::totally_ordered_with<T, F>)
	void assert_less_equal(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (!(actual <= expected))
		{
			throw assert_exception{std::format("assert_less_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_equal(InputIterator1 actual1, InputIterator1 end1, InputIterator2 actual2, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (!std::equal(actual1, end1, actual2))
		{
			throw assert_exception{std::format("assert_sequence_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
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
	void assert_sequence_equal(Range1&& r1, Range2&& r2, std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (!std::ranges::equal(r1, r2, std::ranges::equal_to, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_almost_equal(InputIterator1 actual1, InputIterator1 end1, InputIterator2 actual2, 
		decltype(std::abs(std::declval<std::iter_value_t<InputIterator1>>() - std::declval<std::iter_value_t<InputIterator2>>())) delta,
		std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		for(; actual1 != end1; ++actual1, ++actual2)
		{
			const auto actual = *actual1;
			const auto expected = *actual2;
			const auto difference = std::abs(actual - expected);
			if (difference > delta)
			{
				throw assert_exception{std::format("assert_sequence_almost_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
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
		std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::equal(r1, r2, [delta](const auto& actual, const auto& expected) { 
			const auto difference = std::abs(actual - expected);
			return difference <= delta;
		}, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_almost_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_not_almost_equal(InputIterator1 actual1, InputIterator1 end1, InputIterator2 actual2, decltype(std::abs(*std::declval<InputIterator1>() - *std::declval<InputIterator2>())) delta, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		for(; actual1 != end1; ++actual1, ++actual2)
		{
			const auto actual = *actual1;
			const auto expected = *actual2;
			const auto difference = std::abs(actual - expected);
			if (difference <= delta)
			{
				throw assert_exception{std::format("assert_sequence_not_almost_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
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
		std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::equal(r1, r2, [delta](const auto& actual, const auto& expected) { 
			const auto difference = std::abs(actual - expected);
			return difference > delta;
		}, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_not_almost_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_not_equal(InputIterator1 actual1, InputIterator1 end1, InputIterator2 actual2, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (std::equal(actual1, end1, actual2))
		{
			throw assert_exception{std::format("assert_sequence_not_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
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
	void assert_sequence_not_equal(Range1&& r1, Range2&& r2, std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::equal(r1, r2, std::ranges::equal_to, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_not_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}
}