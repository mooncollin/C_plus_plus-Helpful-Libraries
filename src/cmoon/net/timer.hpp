#pragma once

#include <chrono>
#include <system_error>
#include <thread>

#include "cmoon/net/executor.hpp"
#include "cmoon/net/iocontext.hpp"

namespace cmoon
{
	namespace net
	{
		template<class Clock>
		struct wait_traits
		{
			static typename Clock::duration to_wait_duration(const typename Clock::duration& d)
			{
				return d;
			}

			static typename Clock::duration to_wait_duration(const typename Clock::time_point& t)
			{
				const auto now = Clock::now();
				if (now + Clock::duration::max() < t)
				{
					return Clock::duration::max();
				}
				else if (now + Clock::duration::min() > t)
				{
					return Clock::duration::min();
				}

				return t - now;
			}
		};

		template<class Clock, class WaitTraits = wait_traits<Clock>>
		class basic_waitable_timer
		{
			public:
				using executor_type = io_context::executor_type;
				using clock_type = Clock;
				using duration = typename clock_type::duration;
				using time_point = typename clock_type::time_point;
				using traits_type = WaitTraits;

				explicit basic_waitable_timer(io_context& ctx)
					: basic_waitable_timer(ctx, time_point()) {}

				basic_waitable_timer(io_context& ctx, const time_point& t)
					: ex_{ctx.get_executor()}, t_{t} {}

				basic_waitable_timer(io_context& ctx, const duration& d)
					: ex_{ctx.get_executor()}, t_{clock_type::now() + d} {}

				basic_waitable_timer(const basic_waitable_timer&) = delete;
				basic_waitable_timer(basic_waitable_timer&& rhs)
					ex_{std::move(rhs.ex_)}, t_{std::exchange(rhs.t_, time_point())} {}

				~basic_waitable_timer()
				{
					cancel();
				}

				basic_waitable_timer& operator=(const basic_waitable_timer&) = delete;
				basic_waitable_timer& operator=(basic_waitable_timer&& rhs)
				{
					if (this != std::addressof(rhs))
					{
						rhs.cancel();
						ctx_ = std::move(rhs.ctx_);
						t_ = std::exchange(rhs.t_, time_point());
					}

					return *this;
				}

				executor_type get_executor() noexcept
				{
					return ex_;
				}

				std::size_t cancel()
				{
					return ex_.context().cancel(*this);
				}
				std::size_t cancel_one()
				{
					return ex_.context().cancel_one(*this);
				}

				time_point expiry() const
				{
					return t_;
				}

				std::size_t expires_at(const time_point& t)
				{
					const auto cancelled = cancel();
					t_ = t;
					return cancelled;
				}

				std::size_t expires_after(const duration& d)
				{
					return expires_at(clock_type::now() + d);
				}

				void wait()
				{
					ex_.dispatch([this] {
						while (clock_type::now() < t_)
						{
							std::this_thread::sleep_for(traits_type::to_wait_duration(t_));
						}
					});
				}

				void wait(std::error_code& ec)
				{
					ex_.dispatch([this, &ec] {
						while (!ec && clock_type::now() < t_)
						{
							std::this_thread::sleep_for(traits_type::to_wait_duration(t_));
						}
					});
				}

				template<class CompletionToken>
				details::deduced<CompletionToken, void(std::error_code)> async_wait(CompletionToken&& token)
				{
					async_completion<CompletionToken, void(std::error_code)> completion{token};
					ex_.context().async_wait(*this, std::move(completion.completion_handler));
					return completion.result.get();
				}
			private:
				executor_type ex_;
				time_point t_;
		};

		using system_timer = basic_waitable_timer<std::chrono::system_clock>;
		using steady_timer = basic_waitable_timer<std::chrono::steady_clock>;
		using high_resolution_timer = basic_waitable_timer<std::chrono::high_resolution_clock>;
	}
}