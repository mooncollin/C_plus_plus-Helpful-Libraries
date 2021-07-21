export module cmoon.test.text_test_runner;

import <iostream>;
import <chrono>;
import <iomanip>;
import <type_traits>;
import <vector>;
import <format>;
import <string_view>;
import <iterator>;

import cmoon.utility;
import cmoon.iostream;

import cmoon.test.test_case;
import cmoon.test.test_suite;
import cmoon.test.test_result;
import cmoon.test.runner;

#define INSERTION 0

namespace cmoon::test
{
	export
	template<class Stream>
		requires(requires(Stream s) { cmoon::fprint(s, ""); })
	class text_test_runner : public test_runner
	{
		public:
			text_test_runner(Stream& out)
				requires (!std::is_pointer_v<Stream>)
				: out_{out} {}

			text_test_runner(Stream out)
				requires (std::is_pointer_v<Stream>)
				: out_{out} {}

			bool run(test_case& t_case) override
			{
				if (!t_case.name().empty())
				{
					cmoon::fprint(get_stream(), "\nRunning test case: {}\n", t_case.name());
				}
				print_header();
				cmoon::fprint(get_stream(), "\n\n");

				const auto result = t_case.run();
				print_test_result(result);

				cmoon::fprint(get_stream(), "=======================================\n");
				print_footer();

				if (result.passed())
				{
					cmoon::fprint(get_stream(), "\nOK! 1 passed.\n");
				}
				else
				{
					cmoon::fprint(get_stream(), "\nFAILED! 1 ");
					if (!result.errors().empty())
					{
						cmoon::fprint(get_stream(), "error.\n");
					}
					else
					{
						cmoon::fprint(get_stream(), "failure.\n");
					}
				}

				cmoon::fprint(get_stream(), '\n');
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
					#if INSERTION
					get_stream() << std::format("Running test suite: {}\n", t_suite.name());
					#else
					cmoon::fprint(get_stream(), "Running test suite: {}\n", t_suite.name());
					#endif
				}
				print_header();
				#if INSERTION
				get_stream() << "\n\n";
				#else
				cmoon::fprint(get_stream(), "\n\n");
				#endif

				for (auto& t_case : t_suite)
				{
					const auto result = t_case->run();
					if (!result.errors().empty())
					{
						num_errored++;
						cmoon::fprint(get_stream(), 'E');
						results.emplace_back(result, t_case->name());
					}
					else if (!result.failures().empty())
					{
						num_failed++;
						cmoon::fprint(get_stream(), 'F');
						results.emplace_back(result, t_case->name());
					}
					else
					{
						num_passed++;
						#if INSERTION
						get_stream() << '.';
						#else
						cmoon::fprint(get_stream(), '.');
						#endif
					}
				}

				#if INSERTION
				get_stream() << "\n\n=======================================\n";
				#else
				cmoon::fprint(get_stream(), "\n\n=======================================\n");
				#endif

				for (const auto& [result, name] : results)
				{
					cmoon::fprint(get_stream(), "\n{}:\n", name);
					print_test_result(result);
				}

				#if INSERTION
				get_stream() << "=======================================\n";
				#else
				cmoon::fprint(get_stream(), "=======================================\n");
				#endif
				print_footer();

				if (results.empty())
				{
					#if INSERTION
					get_stream() << "\nOK!";
					#else
					cmoon::fprint(get_stream(), "\nOK!");
					#endif
				}
				else
				{
					cmoon::fprint(get_stream(), "\nFAILED!");
				}

				if (num_passed != 0)
				{
					#if INSERTION
					get_stream() << std::format(" {} passed.", num_passed);
					#else
					cmoon::fprint(get_stream(), " {} passed.", num_passed);
					#endif
				}
				if (num_errored != 0)
				{
					cmoon::fprint(get_stream(), " {} errored.", num_errored);
				}
				if (num_failed != 0)
				{
					cmoon::fprint(get_stream(), " {} failed.", num_failed);
				}
				#if INSERTION
				get_stream() << '\n';
				#else
				cmoon::fprint(get_stream(), '\n');
				#endif

				return results.empty();
			}
		private:
			void print_header()
			{
				stopwatch.reset();
				#if INSERTION
				get_stream() << std::format("Starting testing at {:%F %T}", stopwatch.get_start_time());
				#else
				cmoon::fprint(get_stream(), "Starting testing at {:%F %T}", stopwatch.get_start_time());
				#endif
			}

			void print_footer()
			{
				const auto duration = stopwatch.get_elapsed_time();
				#if INSERTION
				get_stream() << std::format("Ending testing at {:%F %T} : ", stopwatch.get_start_time() + duration);
				#else
				cmoon::fprint(get_stream(), "Ending testing at {:%F %T} : ", stopwatch.get_start_time() + duration);
				#endif

				if (const auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration); 
					seconds.count() > 1)
				{
					cmoon::fprint(get_stream(), "{:.3f}s", seconds.count());
				}
				else if (const auto milliseconds = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration);
						 milliseconds.count() > 1)
				{
					#if INSERTION
					get_stream() << std::format("{:.3f}ms", milliseconds.count());
					#else
					cmoon::fprint(get_stream(), "{:.3f}ms", milliseconds.count());
					#endif
				}
				else
				{
					const auto nanoseconds = std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(duration);
					cmoon::fprint(get_stream(), "{:.3f}ns", nanoseconds.count());
				}
			}

			void print_test_result(const test_result& result)
			{
				if (!result.errors().empty())
				{
					cmoon::fprint(get_stream(), "Errors:\n");

					for (const auto& error : result.errors())
					{
						cmoon::fprint(get_stream(), error.what());
						cmoon::fprint(get_stream(), '\n');
					}
				}

				if (!result.failures().empty())
				{
					cmoon::fprint(get_stream(), "Failures:\n");

					for (const auto& failure : result.failures())
					{
						cmoon::fprint(get_stream(), failure.what());
						cmoon::fprint(get_stream(), '\n');
					}
				}
			}

			[[nodiscard]] inline decltype(auto) get_stream() noexcept
			{
				if constexpr (std::is_pointer_v<Stream>)
				{
					return out_;
				}
				else
				{
					return out_.get();
				}
			}

			std::conditional_t<std::is_pointer_v<Stream>,
							   Stream,
							   std::reference_wrapper<Stream>> out_;
			cmoon::stopwatch stopwatch;
	};
}