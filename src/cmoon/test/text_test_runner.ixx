export module cmoon.test.text_test_runner;

import <iostream>;
import <chrono>;
import <iomanip>;
import <type_traits>;
import <vector>;
import <format>;
import <iterator>;

import cmoon.utility;
import cmoon.iostream;

import cmoon.test.test_case;
import cmoon.test.test_suite;
import cmoon.test.test_result;
import cmoon.test.runner;

namespace cmoon::test
{
	export
	class text_test_runner : public test_runner
	{
		public:
			text_test_runner(std::ostream& out)
				: out_{out} {}

			bool run(test_case& t_case) override
			{
				if (!t_case.name().empty())
				{
					cmoon::fprint(out_.get(), "\nRunning test case: {}\n", t_case.name());
				}
				print_header();
				cmoon::fprint(out_.get(), "\n\n");

				const auto result = t_case.run();
				print_test_result(result);

				cmoon::fprint(out_.get(), "=======================================\n");
				print_footer();

				if (result.passed())
				{
					cmoon::fprint(out_.get(), "\nOK! 1 passed.\n");
				}
				else
				{
					cmoon::fprint(out_.get(), "\nFAILED! 1 ");
					if (!result.errors().empty())
					{
						cmoon::fprint(out_.get(), "error.\n");
					}
					else
					{
						cmoon::fprint(out_.get(), "failure.\n");
					}
				}

				out_.get() << std::endl;
				return result.passed();
			}

			bool run(test_suite& t_suite) override
			{
				std::vector<std::pair<test_result, std::string_view>> results;
				std::size_t num_passed {0};
				std::size_t num_errored {0};
				std::size_t num_failed {0};

				if (!t_suite.name().empty())
				{
					cmoon::fprint(out_.get(), "\nRunning test suite: {}\n", t_suite.name());
				}
				print_header();
				cmoon::fprint(out_.get(), "\n\n");

				for (auto& t_case : t_suite)
				{
					const auto result = t_case->run();
					if (!result.errors().empty())
					{
						num_errored++;
						cmoon::fprint(out_.get(), 'E');
						results.emplace_back(result, t_case->name());
					}
					else if (!result.failures().empty())
					{
						num_failed++;
						cmoon::fprint(out_.get(), 'F');
						results.emplace_back(result, t_case->name());
					}
					else
					{
						num_passed++;
						cmoon::fprint(out_.get(), '.');
					}
				}

				cmoon::fprint(out_.get(), "\n\n=======================================\n");

				for (const auto& [result, name] : results)
				{
					cmoon::fprint(out_.get(), "\n{}:\n", name);
					print_test_result(result);
				}

				cmoon::fprint(out_.get(), "=======================================\n");
				print_footer();

				if (results.empty())
				{
					cmoon::fprint(out_.get(), "\nOK!");
				}
				else
				{
					cmoon::fprint(out_.get(), "\nFAILED!");
				}

				if (num_passed != 0)
				{
					cmoon::fprint(out_.get(), " {} passed.", num_passed);
				}
				if (num_errored != 0)
				{
					cmoon::fprint(out_.get(), " {} errored.", num_errored);
				}
				if (num_failed != 0)
				{
					cmoon::fprint(out_.get(), " {} failed.", num_failed);
				}
				cmoon::fprint(out_.get(), '\n');

				return results.empty();
			}
		private:
			void print_header()
			{
				stopwatch.reset();
				cmoon::fprint(out_.get(), "Starting testing at {:%F %T}", stopwatch.get_start_time());
			}

			void print_footer()
			{
				const auto duration = stopwatch.get_elapsed_time();
				cmoon::fprint(out_.get(), "Ending testing at {:%F %T} : ", stopwatch.get_start_time() + duration);

				if (const auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration); 
					seconds.count() > 1)
				{
					out_.get() << seconds;
				}
				else if (const auto milliseconds = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration);
						 milliseconds.count() > 1)
				{
					out_.get() << milliseconds;
				}
				else
				{
					const auto nanoseconds = std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(duration);
					out_.get() << nanoseconds;
				}
			}

			void print_test_result(const test_result& result)
			{
				if (!result.errors().empty())
				{
					cmoon::fprint(out_.get(), "Errors:\n");

					for (const auto& error : result.errors())
					{
						cmoon::fprint(out_.get(), "{}\n", error.what());
					}
				}

				if (!result.failures().empty())
				{
					cmoon::fprint(out_.get(), "Failures:\n");

					for (const auto& failure : result.failures())
					{
						cmoon::fprint(out_.get(), "{}\n", failure.what());
					}
				}
			}

			std::reference_wrapper<std::ostream> out_;
			cmoon::stopwatch stopwatch;
	};
}