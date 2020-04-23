#pragma once

#include <functional>
#include <chrono>
#include <optional>
#include <future>

namespace collin
{
	namespace futures
	{
		template<class Rep, class Period, class Function, class... Args>
		std::optional<std::result_of_t<Function(Args...)>> wait_for(const std::chrono::duration<Rep, Period>& timeout, Function f, Args... args)
		{
			auto future = std::async(std::launch::async, f, std::forward<Args>(args)...);
			switch (future.wait_for(timeout))
			{
				case std::future_status::timeout:
					return {};
			}

			return future.get();
		}

		template<class Clock, class Duration, class Function, class... Args>
		std::optional<std::result_of_t<Function(Args...)>> wait_until(const std::chrono::time_point<Clock, Duration>& timeout, Function f, Args... args)
		{
			auto future = std::async(std::launch::async, f, std::forward<Args>(args)...);
			switch (future.wait_until(timeout))
			{
				case std::future_status::timeout:
					return {};
			}

			return future.get();
		}
	}
}