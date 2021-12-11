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
import cmoon.execution.sender;

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

	template<sender S, class R>
	struct async_impl
	{
		template<class R>
		struct wrap_r
		{
			public:
				wrap_r(async_impl* impl, std::promise<R>&& p) noexcept
					: impl{impl}, promise_{std::move(p)} {}

				template<class T>
				friend void tag_invoke(set_value_t, wrap_r&& r, T&& v) noexcept
				{
					try
					{
						r.promise_.set_value(std::forward<T>(v));
					}
					catch (...)
					{
						r.promise_.set_exception(std::current_exception());
					}

					delete r.impl;
				}

				template<class E>
				friend void tag_invoke(set_error_t, wrap_r&& r, E&& e) noexcept
				{
					r.promise_.set_exception(std::make_exception_ptr(std::forward<E>(e)));
					delete r.impl;
				}

				friend void tag_invoke(set_done_t, wrap_r&& r) noexcept
				{
					r.promise_.set_exception(std::make_exception_ptr(future_done_exception{}));
					delete r.impl;
				}
			private:
				async_impl* impl;
				std::promise<R> promise_;
		};
		connect_result_t<S, wrap_r<R>> state;

		async_impl(S&& s, std::promise<R>&& p)
			: state{execution::connect(std::forward<S>(s), wrap_r{this, std::move(p)})} {}
	};

	template<sender S, class R>
	async_impl(S&&, std::promise<R>&&) -> async_impl<S, R>;

	export
	template<scheduler Sch, class F, class...Args>
	[[nodiscard]] auto async(Sch&& sch, F&& f, Args&&... args)
	{
		auto work = execution::just(std::forward<Args>(args)...) |
					execution::then(std::forward<F>(f)) |
					execution::on(std::forward<Sch>(sch));

		std::promise<std::invoke_result_t<F, Args...>> p;
		auto future = p.get_future();

		auto impl {new async_impl{std::move(work), std::move(p)}};
		execution::start(impl->state);

		return future;
	}
}