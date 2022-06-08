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
	[[nodiscard]] inline auto output_if_possible(const T& actual, const F& expected)
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

			return std::move(ss).str();
		}
		else
		{
			return "";
		}
	}

	export
	void fail(std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		throw assert_exception{std::format("failure at line {} ({}): {}", location.line(), location.file_name(), message)};
	}

	export
	void error(std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		throw std::runtime_error{std::format("error at line {} ({}): {}", location.line(), location.file_name(), message)};
	}

	export
	template<class T, class F>
		requires(requires(const T& t, const F& f) {
			{ t == f } -> std::convertible_to<bool>;
		})
	void assert_equal(const T& actual, const F& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
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
	template<class T>
	void assert_is(const T& actual, const T& expected, std::string_view message = "", const std::source_location& location = std::source_location::current())
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
		const auto casted {dynamic_cast<const Expected*>(std::addressof(actual))};
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
	template<class T, std::ranges::input_range R>
	void assert_in(const T& member, R&& r, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::find(r, member) == std::ranges::end(r))
		{
			throw assert_exception{std::format("assert_in failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<class T, std::ranges::forward_range R>
	void assert_not_in(const T& member, R&& r, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::find(r, member) != std::end(r))
		{
			throw assert_exception{std::format("assert_not_in failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<class T, class F, class... Args>
		requires(std::invocable<F, Args...>)
	void assert_throws(F&& callable, std::string_view message, Args&&... args, const std::source_location& location = std::source_location::current())
	{
		try
		{
			std::forward<F>(callable)(std::forward<Args>(args)...);
			return;
		}
		catch (std::decay_t<T>&)
		{
			// Do nothing, it passed.
			return;
		}

		throw assert_exception{std::format("assert_throws failed at line {} ({}): {}\nExpected {} to be thrown, but didn't", location.line(), location.file_name(), message, typeid(T).name())};
	}

	export
	template<class T, class F, class... Args>
		requires(std::invocable<F, Args...>)
	void assert_throws(F&& callable, Args&&... args, const std::source_location& location = std::source_location::current())
	{
		assert_throws<T>(callable, std::string_view{""}, std::forward<Args>(args)..., location);
	}

	export
	template<cmoon::arithmetic T, cmoon::arithmetic F>
	void assert_almost_equal(const T& actual, const F& expected, const std::common_type_t<T, F>& delta, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		const auto difference {std::abs(actual - expected)};
		if (difference > delta)
		{
			if constexpr (cmoon::formattable<decltype(difference)>)
			{
				throw assert_exception{std::format("assert_almost_equal failed at line {} ({}): {}\n{}Difference: {}\n", location.line(), location.file_name(), message, output_if_possible(actual, expected), difference)};
			}
			else
			{
				throw assert_exception{std::format("assert_almost_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
			}
		}
	}

	export
	template<cmoon::arithmetic T, cmoon::arithmetic F>
	void assert_not_almost_equal(const T& actual, const F& expected, const std::common_type_t<T, F>& delta, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		const auto difference = std::abs(actual - expected);
		if (difference <= delta)
		{
			if constexpr (cmoon::formattable<decltype(difference)>)
			{
				throw assert_exception{std::format("assert_not_almost_equal failed at line {} ({}): {}\n{}Difference: {}\n", location.line(), location.file_name(), message, output_if_possible(actual, expected), difference)};
			}
			else
			{
				throw assert_exception{std::format("assert_not_almost_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
			}
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
				class Pred = std::ranges::equal_to,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											Pred,
											Proj1,
											Proj2>)
	void assert_sequence_equal(Range1&& r1, Range2&& r2, std::string_view message = "", Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (!std::ranges::equal(r1, r2, pred, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
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
				class Pred = std::ranges::not_equal_to,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											Pred,
											Proj1,
											Proj2>)
	void assert_sequence_not_equal(Range1&& r1, Range2&& r2, std::string_view message = "", Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::equal(r1, r2, pred, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_not_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2, class D>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_almost_equal(InputIterator1 actual1, InputIterator1 end1, InputIterator2 actual2, 
		const D& delta,
		std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		for(; actual1 != end1; ++actual1, ++actual2)
		{
			const decltype(auto) actual {*actual1};
			const decltype(auto) expected {*actual2};
			const auto difference {std::abs(actual - expected)};
			if (difference > delta)
			{
				throw assert_exception{std::format("assert_sequence_almost_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
			}
		}
	}

	export
	template<std::ranges::input_range Range1, std::ranges::input_range Range2, class D,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											std::equal_to<>,
											Proj1,
											Proj2>)
	void assert_sequence_almost_equal(Range1&& r1, Range2&& r2, 
		const D& delta,
		std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (!std::ranges::equal(r1, r2, [&delta](const auto& actual, const auto& expected) { 
			const auto difference {std::abs(actual - expected)};
			return difference <= delta;
		}, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_almost_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}

	export
	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2, class D>
		requires(std::indirectly_comparable<InputIterator1, InputIterator2, std::equal_to<>>)
	void assert_sequence_not_almost_equal(InputIterator1 actual1, InputIterator1 end1, InputIterator2 actual2, const D& delta, std::string_view message = "", const std::source_location& location = std::source_location::current())
	{
		for(; actual1 != end1; ++actual1, ++actual2)
		{
			const auto actual {*actual1};
			const auto expected {*actual2};
			const auto difference {std::abs(actual - expected)};
			if (difference <= delta)
			{
				throw assert_exception{std::format("assert_sequence_not_almost_equal failed at line {} ({}): {}\n{}", location.line(), location.file_name(), message, output_if_possible(actual, expected))};
			}
		}
	}

	export
	template<std::ranges::input_range Range1, std::ranges::input_range Range2, class D,
				class Proj1 = std::identity, class Proj2 = std::identity>
		requires(std::indirectly_comparable<std::ranges::iterator_t<Range1>, 
											std::ranges::iterator_t<Range2>, 
											std::equal_to<>,
											Proj1,
											Proj2>)
	void assert_sequence_not_almost_equal(Range1&& r1, Range2&& r2, 
		const D& delta,
		std::string_view message = "", Proj1 proj1 = {}, Proj2 proj2 = {}, const std::source_location& location = std::source_location::current())
	{
		if (std::ranges::equal(r1, r2, [&delta](const auto& actual, const auto& expected) { 
			const auto difference {std::abs(actual - expected)};
			return difference > delta;
		}, proj1, proj2))
		{
			throw assert_exception{std::format("assert_sequence_not_almost_equal failed at line {} ({}): {}\n", location.line(), location.file_name(), message)};
		}
	}
}