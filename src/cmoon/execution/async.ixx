export module cmoon.execution.async;

import <future>;
import <concepts>;
import <utility>;
import <exception>;

import cmoon.execution.scheduler;
import cmoon.execution.then;
import cmoon.execution.transfer_just;
import cmoon.execution.done_as_error;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.start;
import cmoon.execution.connect;
import cmoon.execution.receiver;
import cmoon.execution.schedule;
import cmoon.execution.on;
import cmoon.execution.just;

namespace cmoon::execution
{
	export
	class future_done_exception : public std::exception
	{
		public:
			future_done_exception() = default;

			const char* what() const noexcept final
			{
				return "future cancelled";
			}
	};

	template<class R>
	struct async_receiver
	{
		public:
			async_receiver(std::promise<R>&& p) noexcept
				: promise_{std::move(p)} {}

			template<class T>
			friend void tag_invoke(set_value_t, async_receiver&& r, T&& v) noexcept
			{
				try
				{
					r.promise_.set_value(std::forward<T>(v));
				}
				catch (...)
				{
					r.promise_.set_exception(std::current_exception());
				}
			}

			template<class E>
			friend void tag_invoke(set_error_t, async_receiver&& r, E&& e) noexcept
			{
				r.promise_.set_exception(std::make_exception_ptr(std::forward<E>(e)));
			}

			friend void tag_invoke(set_done_t, async_receiver&& r) noexcept
			{
				std::terminate();
			}
		private:
			std::promise<R> promise_;
	};

	export
	template<scheduler Sch, class F, class...Args>
	[[nodiscard]] auto async(Sch&& sch, F&& f, Args&&... args)
	{
		auto work = execution::just(std::forward<Args>(args)...) |
					execution::then(std::forward<F>(f)) |
					execution::done_as_error(future_done_exception{}) |
					execution::on(std::forward<Sch>(sch));

		std::promise<std::invoke_result_t<F, Args...>> p;
		auto future = p.get_future();

		execution::start(
			execution::connect(
				std::move(work),
				async_receiver{std::move(p)}
			)
		);

		return future;
	}
}