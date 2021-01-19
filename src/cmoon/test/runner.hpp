#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <type_traits>
#include <vector>

#include "cmoon/format.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

namespace cmoon
{
	namespace test
	{
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

				text_test_runner(const text_test_runner&) = delete;
				text_test_runner(text_test_runner&&) = delete;

				text_test_runner& operator=(const text_test_runner&) = delete;
				text_test_runner& operator=(text_test_runner&&) = delete;

				bool run(test_case& t_case) override
				{
					const auto time_now = print_header();
					out_.get() << '\n';

					out_.get() << cmoon::format("Running test case: {}\n", t_case.name());

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
					std::vector<std::pair<test_result, std::string_view>> results;
					std::size_t num_passed {0};
					std::size_t num_errored {0};
					std::size_t num_failed {0};

					const auto time_now = print_header();
					out_.get() << "\n\n";

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
						out_.get() << cmoon::format("\n{}:\n", name);
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
						out_.get() << cmoon::format("{} passed.", num_passed);
					}
					if (num_errored != 0)
					{
						out_.get() << cmoon::format("{} errored.", num_errored);
					}
					if (num_failed != 0)
					{
						out_.get() << cmoon::format("{} failed.", num_failed);
					}

					out_.get() << std::endl;
					return results.empty();
				}
			private:
				std::chrono::system_clock::time_point print_header()
				{
					const auto time_now = std::chrono::system_clock::now();
					const auto t_c = std::chrono::system_clock::to_time_t(time_now);
					::tm t;
					::localtime_s(std::addressof(t), std::addressof(t_c));
					out_.get() << "Starting testing at " << std::put_time(std::addressof(t), "%F %T");

					return time_now;
				}

				void print_footer(const std::chrono::system_clock::time_point& before)
				{
					const auto time_now = std::chrono::system_clock::now();
					const auto duration = time_now - before;
					const auto t_c = std::chrono::system_clock::to_time_t(time_now);
					::tm t;
					::localtime_s(std::addressof(t), std::addressof(t_c));

					out_.get() << "Ending testing at " << std::put_time(std::addressof(t), "%F %T");
					out_.get() << " : ";

					const auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration);
					const auto milliseconds = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration);
					const auto nanoseconds = std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(duration);
					if (seconds.count() > 1)
					{
						out_.get() << seconds.count();
						out_.get() << "s";
					}
					else if(milliseconds.count() > 1)
					{
						out_.get() << milliseconds.count();
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
	}
}