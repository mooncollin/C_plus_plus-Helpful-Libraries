export module cmoon.test.text_test_runner;

import <ostream>;
import <chrono>;
import <iomanip>;
import <type_traits>;
import <vector>;
import <format>;
import <string_view>;
import <iterator>;

import cmoon.utility;
import cmoon.scope;
import cmoon.concepts;
import cmoon.ranges;

import cmoon.test.test_case;
import cmoon.test.test_suite;
import cmoon.test.test_result;

namespace cmoon::test
{
	export
	class text_test_runner
	{
		public:
			text_test_runner(std::ostream& out) noexcept
				: output_{out} {}

			test_result<> run(test_case& t_case)
			{
				if (!t_case.name().empty())
				{
					output_ << "\nRunning test case: ";
					output_ << t_case.name();
					output_ << '\n';
				}
				print_header();
				stopwatch.reset();
				const auto result {t_case.run()};
				const auto duration {stopwatch.get_elapsed_time()};
				print_test_result(result);

				output_ << "=======================================\n";
				print_footer(duration);

				if (result.passed())
				{
					output_ << "\nOK! 1 passed.\n\n";
				}
				else
				{
					output_ << "\nFAILED! 1 ";
					if (!result.errors().empty())
					{
						output_ << "error.\n\n";
					}
					else
					{
						output_ << "failure.\n\n";
					}
				}

				return result;
			}

			template<class Allocator>
			std::vector<test_result<Allocator>, typename std::allocator_traits<Allocator>::template rebind_alloc<test_result<Allocator>>>
			run(test_suite<Allocator>& t_suite)
			{
				std::vector<test_result<Allocator>, typename std::allocator_traits<Allocator>::template rebind_alloc<test_result<Allocator>>> results;
				std::size_t num_passed {0};
				std::size_t num_errored {0};
				std::size_t num_failed {0};

				if (!t_suite.name().empty())
				{
					output_ << "Running test suite: ";
					output_ << t_suite.name();
					output_ << '\n';
				}
				print_header();
				stopwatch.reset();
				for (auto t_case : t_suite)
				{
					results.push_back(t_case->run(t_suite.get_allocator()));
					if (!results.back().errors().empty())
					{
						num_errored++;
						output_ << 'E';
					}
					else if (!results.back().failures().empty())
					{
						num_failed++;
						output_ << 'F';
					}
					else
					{
						num_passed++;
						output_ << '.';
					}
				}
				const auto duration {stopwatch.get_elapsed_time()};

				output_ << "\n\n=======================================\n";

				for (const auto& [result, test_case] : cmoon::ranges::views::zip(results, t_suite))
				{
					if (!result.passed())
					{
						output_ << '\n';
						output_ << test_case->name();
						output_ << '\n';
						print_test_result(result);
					}
				}

				output_ << "=======================================\n";
				print_footer(duration);

				if (num_passed == std::size(results))
				{
					output_ << "\nOK!";
				}
				else
				{
					output_ << "\nFAILED!";
				}

				if (num_passed != 0)
				{
					output_ << ' ';
					output_ << num_passed;
					output_ << " passed";
				}
				if (num_errored != 0)
				{
					output_ << ' ';
					output_ << num_errored;
					output_ << " errored";
				}
				if (num_failed != 0)
				{
					output_ << ' ';
					output_ << num_failed;
					output_ << " failed";
				}
				output_ << '\n';

				return results;
			}
		private:
			void print_header()
			{
				output_ << "Starting testing at ";
				output_ << std::format("{:%F %T}", cmoon::stopwatch::clock_t::now());
				output_ << "\n\n";
			}

			template<class Duration>
			void print_footer(const Duration& duration)
			{
				output_ << "Ending testing at ";
				output_ << std::format("{:%F %T}", stopwatch.get_start_time() + duration);
				output_ << " : ";

				const auto before_fmt {output_.flags()};
				cmoon::scope_exit reset_fmt {[this, &before_fmt] {
					output_.setf(before_fmt);
				}};
				output_.setf(std::ios::fixed);
				output_.precision(3);

				if (const auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration); 
					seconds.count() > 1)
				{
					output_ << seconds;
				}
				else if (const auto milliseconds = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration);
						 milliseconds.count() > 1)
				{
					output_ << milliseconds;
				}
				else
				{
					const auto nanoseconds = std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(duration);
					output_ << nanoseconds;
				}
			}

			template<class Allocator>
			void print_test_result(const test_result<Allocator>& result)
			{
				if (!result.errors().empty())
				{
					output_ << "Errors:\n";

					for (const auto& error : result.errors())
					{
						output_ << error.what()
								<< '\n';
					}
				}

				if (!result.failures().empty())
				{
					output_ << "Failures:\n";

					for (const auto& failure : result.failures())
					{
						output_ << failure.what()
								<< '\n';
					}
				}
			}

			std::ostream& output_;
			cmoon::stopwatch stopwatch;
	};
}