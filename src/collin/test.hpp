#pragma once

#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <tuple>
#include <initializer_list>
#include <functional>
#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <chrono>
#include <iomanip>
#include <ratio>

#include "collin/concepts.hpp"

namespace collin
{
	namespace test
	{
		class assert_exception : public std::runtime_error
		{
			public:
				assert_exception(std::string_view str)
					: std::runtime_error{std::string(str)} {}
		};

		class test_result
		{
			public:
				void add_error(const std::exception& e)
				{
					errors_.push_back(e);
				}

				void add_error(std::exception&& e)
				{
					errors_.push_back(std::move(e));
				}

				void add_failure(const std::exception& e)
				{
					failures_.push_back(e);
				}

				void add_failure(std::exception&& e)
				{
					failures_.push_back(std::move(e));
				}

				const std::vector<std::exception>& errors() const
				{
					return errors_;
				}

				const std::vector<std::exception>& failures() const
				{
					return failures_;
				}

				bool passed() const noexcept
				{
					return errors_.empty() && failures_.empty();
				}
			private:
				std::vector<std::exception> errors_;
				std::vector<std::exception> failures_;
		};

		class test_case
		{
			public:
				test_case() = default;

				test_case(std::string_view str)
					: name_{str} {}

				virtual void set_up() {}

				test_result run()
				{
					test_result result;

					#ifndef _DEBUG
					try
					{
					#endif

						set_up();
						operator()();

					#ifndef _DEBUG
					}
					catch (const assert_exception& e)
					{
						result.add_failure(e);
					}
					catch (const std::exception& e)
					{
						result.add_error(e);
					}
					#endif

					return result;
				}

				virtual void operator()() = 0;

				const std::string& name() const noexcept
				{
					return name_;
				}

				void name(std::string_view str)
				{
					name_ = str;
				}

			private:
				std::string name_ {};
		};

		class test_suite
		{
			class test_suite_iterator;
			using container = std::vector<std::unique_ptr<test_case>>;
			using iterator = typename container::iterator;

			public:
				template<class T, class... Args>
				void add_test_case(Args&&... args)
				{
					cases.push_back(std::make_unique<T>(std::forward<Args>(args)...));
				}

				void add_test_suite(test_suite&& suite)
				{
					for (auto&& case_ : suite.cases)
					{
						cases.push_back(std::move(case_));
					}

					suite.clear();
				}

				std::vector<test_result> run_all()
				{
					std::vector<test_result> results;
					for (auto& t_case : cases)
					{
						results.push_back(t_case->run());
					}

					return results;
				}

				iterator begin()
				{
					return std::begin(cases);
				}

				iterator end()
				{
					return std::end(cases);
				}

				void clear()
				{
					cases.clear();
				}
			private:
				container cases;
		};

		class test_runner
		{
			public:
				test_runner() = default;
				test_runner(const test_runner&) = delete;
				test_runner(test_runner&&) = delete;

				test_runner& operator=(const test_runner&) = delete;
				test_runner& operator=(test_runner&&) = delete;

				virtual ~test_runner() noexcept = default;

				virtual bool run(test_case&) = 0;
				virtual bool run(test_suite&) = 0;
		};

		class text_test_runner : public test_runner
		{
			public:
				text_test_runner(std::ostream& out)
					: out_{out} {}

				bool run(test_case& t_case) override
				{
					const auto time_now = print_header();
					out_.get() << '\n';

					out_.get() << "Running test case: " << t_case.name() << '\n';

					const auto result = t_case.run();
					print_test_result(result);

					out_.get() << "=======================================\n";
					print_footer(time_now);
					out_.get() << '\n';

					if (result.passed())
					{
						out_.get() << "OK! 1 passed.\n";
					}
					else
					{
						out_.get() << "FAILED! 1 ";
						if (!result.errors().empty())
						{
							out_.get() << "error";
						}
						else
						{
							out_.get() << "failure";
						}
						out_.get() << ".\n";
					}

					out_.get() << std::endl;
					return result.passed();
				}

				bool run(test_suite& t_suite) override
				{
					const auto time_now = print_header();
					out_.get() << "\n\n";

					std::vector<std::pair<test_result, std::string_view>> results;
					std::size_t num_passed {0};
					std::size_t num_errored {0};
					std::size_t num_failed {0};

					for (auto& t_case : t_suite)
					{
						const auto result = t_case->run();
						if (!result.errors().empty())
						{
							num_errored++;
							out_.get() << 'E';
							results.emplace_back(result, t_case->name());
						}
						else if (!result.failures().empty())
						{
							num_failed++;
							out_.get() << 'F';
							results.emplace_back(result, t_case->name());
						}
						else
						{
							num_passed++;
							out_.get() << '.';
						}
					}

					out_.get() << "\n\n=======================================\n";

					for (const auto& [result, name] : results)
					{
						out_.get() << '\n' << name << ":\n";
						print_test_result(result);
					}

					out_.get() << "=======================================\n";
					print_footer(time_now);
					out_.get() << '\n';

					if (results.empty())
					{
						out_.get() << "OK! ";
					}
					else
					{
						out_.get() << "FAILED! ";
					}

					if (num_passed != 0)
					{
						out_.get() << num_passed << " passed. ";
					}
					if (num_errored != 0)
					{
						out_.get() << num_errored << " errored. ";
					}
					if (num_failed != 0)
					{
						out_.get() << num_failed << " failed.";
					}

					out_.get() << std::endl;
					return results.empty();
				}
			private:
				std::chrono::system_clock::time_point print_header()
				{
					const auto time_now = std::chrono::system_clock::now();
					const auto t_c = std::chrono::system_clock::to_time_t(time_now);
					out_.get() << "Starting testing at " << std::put_time(std::localtime(&t_c), "%F %T");

					return time_now;
				}

				void print_footer(const std::chrono::system_clock::time_point& before)
				{
					const auto time_now = std::chrono::system_clock::now();
					const auto t_c = std::chrono::system_clock::to_time_t(time_now);
					const auto duration = time_now - before;


					out_.get() << "Ending testing at " << std::put_time(std::localtime(&t_c), "%F %T");
					out_.get() << " : ";

					const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
					const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
					const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
					if (seconds.count() > 0)
					{
						out_.get() << (seconds.count() + (milliseconds.count() / static_cast<double>(std::milli::den)));
						out_.get() << "s";
					}
					else if(milliseconds.count() > 0)
					{
						out_.get() << (milliseconds.count() + (nanoseconds.count() / static_cast<double>(std::nano::den)));
						out_.get() << "ms";
					}
					else
					{
						out_.get() << nanoseconds.count();
						out_.get() << "ns";
					}
				}

				void print_test_result(const test_result& result)
				{
					if (!result.errors().empty())
					{
						out_.get() << "Errors:\n";

						for (const auto& error : result.errors())
						{
							out_.get() << error.what() << '\n';
						}
					}

					if (!result.failures().empty())
					{
						out_.get() << "Failures:\n";

						for (const auto& failure : result.failures())
						{
							out_.get() << failure.what() << '\n';
						}
					}
				}

				std::reference_wrapper<std::ostream> out_;
		};

		template<class T, class F>
			requires(collin::concepts::weakly_equality_comparable_with<T, F>)
		void assert_equal(const T& first, const F& second, std::string_view message="")
		{
			if(first != second)
			{
				std::ostringstream ss;
				ss << "assert_equal failed: " << message << '\n';
				if constexpr (concepts::stream_writable<T, decltype(ss)> &&
					concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
			requires(collin::concepts::weakly_equality_comparable_with<T, F>)
		void assert_not_equal(const T& first, const F& second, std::string_view message = "")
		{
			if (first == second)
			{
				std::ostringstream ss;
				ss << "assert_not_equal failed: " << message << '\n';
				if constexpr (collin::concepts::stream_writable<T, decltype(ss)> &&
					collin::concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<collin::concepts::boolean T>
		void assert_true(const T& first, std::string_view message="")
		{
			if (!static_cast<bool>(first))
			{
				std::ostringstream ss;
				ss << "assert_true failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<collin::concepts::boolean T>
		void assert_false(const T& first, std::string_view message = "")
		{
			if (static_cast<bool>(first))
			{
				std::ostringstream ss;
				ss << "assert_false failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
			requires(collin::concepts::weakly_equality_comparable_with<T*, F*>)
		void assert_is(const T& first, const F& second, std::string_view message = "")
		{
			if (std::addressof(first) != std::addressof(second))
			{
				std::ostringstream ss;
				ss << "assert_is failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
			requires(collin::concepts::weakly_equality_comparable_with<T*, F*>)
		void assert_is_not(const T& first, const F& second, std::string_view message = "")
		{
			if (std::addressof(first) == std::addressof(second))
			{
				std::ostringstream ss;
				ss << "assert_is_not failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
		void assert_is_instance(const T& first, std::string_view message = "")
		{
			const auto first_type = typeid(first);
			const auto second_type = typeid(F);
			if (first_type != second_type)
			{
				std::ostringstream ss;
				ss << "assert_is_instance failed: " << message << '\n';
				ss << "First:\n" << first_type.name() << '\n';
				ss << "Second:\n" << second_type.name() << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
		void assert_is_not_instance(const T& first, std::string_view message = "")
		{
			const auto first_type = typeid(first);
			const auto second_type = typeid(F);
			if (first_type == second_type)
			{
				std::ostringstream ss;
				ss << "assert_is_not_instance failed: " << message << '\n';
				ss << "First:\n" << first_type.name() << '\n';
				ss << "Second:\n" << second_type.name() << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
		void assert_in(const T& member, const F& container, std::string_view message = "")
		{
			if (std::find(std::begin(container), std::end(container), member) == std::end(container))
			{
				std::ostringstream ss;
				ss << "assert_in failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
		void assert_not_in(const T& member, const F& container, std::string_view message = "")
		{
			if (std::find(std::begin(container), std::end(container), member) != std::end(container))
			{
				std::ostringstream ss;
				ss << "assert_not_in failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F, class... Args>
			requires(collin::concepts::invocable<F, Args...>)
		void assert_throws(const T& exception, F&& callable, Args&&... args)
		{
			assert_throws(exception, callable, std::string_view{""}, std::forward<Args>(args)...);
		}

		template<class T, class F, class... Args>
			requires(collin::concepts::invocable<F, Args...>)
		void assert_throws(const T& exception, F&& callable, std::string_view message, Args&&... args)
		{
			try
			{
				std::forward<F>(callable)(std::forward<Args>(args)...);
				std::ostringstream ss;
				ss << "assert_throws failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
			catch (decltype(exception) e)
			{
				// Do nothing, it passed.
			}
		}

		template<collin::concepts::arithmetic T, collin::concepts::arithmetic F>
		void assert_almost_equal(const T& first, const F& second, const std::common_type_t<T, F>& delta, std::string_view message = "")
		{
			const auto difference = std::abs(first - second);
			if (difference > delta)
			{
				std::ostringstream ss;
				ss << "assert_almost_equal failed: " << message << '\n';
				if constexpr (concepts::stream_writable<T, decltype(ss)> &&
					concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<collin::concepts::arithmetic T, collin::concepts::arithmetic F>
		void assert_not_almost_equal(const T& first, const F& second, const std::common_type_t<T, F>& delta, std::string_view message = "")
		{
			const auto difference = std::abs(first - second);
			if (difference <= delta)
			{
				std::ostringstream ss;
				ss << "assert_almost_equal failed: " << message << '\n';
				if constexpr (concepts::stream_writable<T, decltype(ss)> &&
					concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				if constexpr (type_traits::is_stream_writable_v<decltype(difference), decltype(ss)>)
				{
					ss << "Difference: " << difference << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
			requires requires(const T& t, const F& f)
			{
				{t > f} -> collin::concepts::boolean;
			}
		void assert_greater(const T& first, const F& second, std::string_view message = "")
		{
			if (!(first > second))
			{
				std::ostringstream ss;
				ss << "assert_greater failed: " << message << '\n';
				if constexpr (concepts::stream_writable<T, decltype(ss)> &&
					concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
			requires requires(const T& t, const F& f)
			{
				{t >= f} -> collin::concepts::boolean;
			}
		void assert_greater_equal(const T& first, const F& second, std::string_view message = "")
		{
			if (!(first >= second)) // Can't know for sure if first < second is supported
			{
				std::ostringstream ss;
				ss << "assert_greater_equal failed: " << message << '\n';
				if constexpr (concepts::stream_writable<T, decltype(ss)> &&
					concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
			requires requires(const T& t, const F& f)
			{
				{t < f} -> collin::concepts::boolean;
			}
		void assert_less(const T& first, const F& second, std::string_view message = "")
		{
			if (!(first < second)) // Can't know for sure if first >= second is supported
			{
				std::ostringstream ss;
				ss << "assert_less failed: " << message << '\n';
				if constexpr (concepts::stream_writable<T, decltype(ss)> &&
					concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
			requires requires(const T& t, const F& f)
			{
				{t <= f} -> collin::concepts::boolean;
			}
		void assert_less_equal(const T& first, const F& second, std::string_view message = "")
		{
			if (!(first <= second)) // Can't know for sure if first > second is supported
			{
				std::ostringstream ss;
				ss << "assert_less_equal failed: " << message << '\n';
				if constexpr (concepts::stream_writable<T, decltype(ss)> &&
					concepts::stream_writable<F, decltype(ss)>)
				{
					ss << "First:\n" << first << '\n';
					ss << "Second:\n" << second << '\n';
				}
				throw assert_exception(ss.str());
			}
		}

		template<collin::concepts::iterator InputIterator1, collin::concepts::iterator InputIterator2>
			requires(collin::concepts::weakly_equality_comparable_with<decltype(*std::declval<InputIterator1>), decltype(*std::declval<InputIterator2>)>)
		void assert_sequence_equal(InputIterator1 first1, InputIterator1 end1, InputIterator2 first2, std::string_view message = "")
		{
			if (!std::equal(first1, end1, first2))
			{
				std::ostringstream ss;
				ss << "assert_sequence_equal failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<collin::concepts::iterator InputIterator1, collin::concepts::iterator InputIterator2>
			requires(collin::concepts::weakly_equality_comparable_with<decltype(*std::declval<InputIterator1>), decltype(*std::declval<InputIterator2>)>)
		void assert_sequence_not_equal(InputIterator1 first1, InputIterator1 end1, InputIterator2 first2, std::string_view message = "")
		{
			if (std::equal(first1, end1, first2))
			{
				std::ostringstream ss;
				ss << "assert_sequence_not_equal failed: " << message << '\n';
				throw assert_exception(ss.str());
			}
		}

		template<class T, class F>
		inline void output_if_possible(const T& first, const F& second, std::ostream& os)
		{
			if constexpr (concepts::stream_writable<T, decltype(os)> &&
				concepts::stream_writable<F, decltype(os)>)
			{
				os << "First:\n" << first << '\n';
				os << "Second:\n" << second << '\n';
			}
		}
	}
}