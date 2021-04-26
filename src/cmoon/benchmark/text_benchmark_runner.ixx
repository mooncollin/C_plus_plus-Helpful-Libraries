module;

#include "cmoon/string/literals.hpp"

export module cmoon.benchmark.text_benchmark_runner;

import <iostream>;

import cmoon.format;
import cmoon.string;
import cmoon.datetime;

import cmoon.benchmark.benchmark_c;
import cmoon.benchmark.suite;

namespace cmoon::benchmark
{
	export
	template<class CharT, class Traits>
	class text_benchmark_runner : public benchmark_runner
	{
		public:
			text_benchmark_runner(std::basic_ostream<CharT, Traits>& out)
				: out_{out} {}

			void run(benchmark& bench) override
			{
				benchmark_suite suite;
				suite.add_benchmark<benchmark>(bench);
				run(suite);
			}

			void run(benchmark_suite& suite) override
			{
				out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("[==========] Running {} benchmark{}.\n")), suite.num_benchmarks(),  plural(suite.num_benchmarks()));

				for (auto& bench : suite)
				{
						
					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("[ RUN      ] {} ({} run{}, {} iteration{} per run)\n")), bench->name(), bench->runs(), plural(bench->runs()), bench->iterations_per_run(), plural(bench->iterations_per_run()));
					const auto results = bench->do_benchmark();

					const auto average_run_time = results.average_run_time();
					const auto fastest_run = results.fastest_run();
					const auto slowest_run = results.slowest_run();
						
					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("[     DONE ] {} (")), bench->name());
					pretty_print_duration(results.total_run_time());
					out_.get() << cmoon::choose_str_literal<CharT>(STR_LITERALS(")\n"));

					out_.get() << cmoon::choose_str_literal<CharT>(STR_LITERALS("[   RUNS   ]        Average time: "));
					pretty_print_duration(average_run_time);
					out_.get() << cmoon::choose_str_literal<CharT>(STR_LITERALS("\n                         Fastest: "));
					pretty_print_duration(fastest_run.total_time());
					out_.get() << cmoon::choose_str_literal<CharT>(STR_LITERALS("\n                         Slowest: "));
					pretty_print_duration(slowest_run.total_time());
					out_.get() << "\n\n";

					const auto run_average_performance = std::chrono::duration<double>{1} / std::chrono::duration_cast<std::chrono::duration<double>>(average_run_time);
					const auto run_best_performance = std::chrono::duration<double>{1} / std::chrono::duration_cast<std::chrono::duration<double>>(fastest_run.total_time());
					const auto run_worst_performance = std::chrono::duration<double>{1} / std::chrono::duration_cast<std::chrono::duration<double>>(slowest_run.total_time());

					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("             Average performance: {:.5f} runs/s\n")), run_average_performance);
					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("                Best performance: {:.5f} runs/s\n")), run_best_performance);
					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("               Worst performance: {:.5f} runs/s\n")), run_worst_performance);

					const auto average_iteration_time = results.average_iteration_time();
					const auto fastest_iteration = results.fastest_iteration();
					const auto slowest_iteration = results.slowest_iteration();

					out_.get() << cmoon::choose_str_literal<CharT>(STR_LITERALS("[ITERATIONS]        Average time: "));
					pretty_print_duration(average_iteration_time);
					out_.get() << cmoon::choose_str_literal<CharT>(STR_LITERALS("\n                         Fastest: "));
					pretty_print_duration(fastest_iteration);
					out_.get() << cmoon::choose_str_literal<CharT>(STR_LITERALS("\n                         Slowest: "));
					pretty_print_duration(slowest_iteration);
					out_.get() << "\n\n";

					const auto iteration_average_performance = std::chrono::duration<double>{1} / std::chrono::duration_cast<std::chrono::duration<double>>(average_iteration_time);
					const auto iteration_best_performance = std::chrono::duration<double>{1} / std::chrono::duration_cast<std::chrono::duration<double>>(fastest_iteration);
					const auto iteration_worst_performance = std::chrono::duration<double>{1} / std::chrono::duration_cast<std::chrono::duration<double>>(slowest_iteration);

					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("             Average performance: {:.5f} iterations/s\n")), iteration_average_performance);
					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("                Best performance: {:.5f} iterations/s\n")), iteration_best_performance);
					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("               Worst performance: {:.5f} iterations/s\n")), iteration_worst_performance);
				}
			}
		private:
			std::reference_wrapper<std::basic_ostream<CharT, Traits>> out_;

			[[nodiscard]] static constexpr std::basic_string_view<CharT> plural(std::size_t s) noexcept
			{
				if (s == 1)
				{
					return cmoon::choose_str_literal<CharT>(STR_LITERALS(""));
				}

				return cmoon::choose_str_literal<CharT>(STR_LITERALS("s"));
			}

			void pretty_print_duration(const std::chrono::nanoseconds& ns)
			{
				const auto ns2 = cmoon::measures::from_chrono(ns);
				const auto hours = cmoon::measures::unit_cast<cmoon::measures::basic_hours<double>>(ns2);
				if (hours.count() > 1)
				{
					out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("{:%Q %q}")), cmoon::measures::to_chrono(hours));
				}
				else
				{
					const auto minutes = cmoon::measures::unit_cast<cmoon::measures::basic_minutes<double>>(ns2);
					if (minutes.count() > 1)
					{
						out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("{:%Q %q}")), cmoon::measures::to_chrono(minutes));
					}
					else
					{
						const auto seconds = cmoon::measures::unit_cast<cmoon::measures::basic_seconds<double>>(ns2);
						if (seconds.count() > 1)
						{
							out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("{:%Q %q}")), cmoon::measures::to_chrono(seconds));
						}
						else
						{
							const auto milliseconds = cmoon::measures::unit_cast<cmoon::measures::basic_milliseconds<double>>(ns2);
							if (milliseconds.count() > 1)
							{
								out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("{:%Q %q}")), cmoon::measures::to_chrono(milliseconds));
							}
							else
							{
								out_.get() << cmoon::format(cmoon::choose_str_literal<CharT>(STR_LITERALS("{:%Q %q}")), ns);
							}
						}
					}
				}
			}
	};
}