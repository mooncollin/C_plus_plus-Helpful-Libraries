export module cmoon.execution.connect;

import <utility>;
import <type_traits>;
import <exception>;
import <coroutine>;
import <functional>;
import <concepts>;

import cmoon.functional;
import cmoon.meta;

import cmoon.execution.impl.is_awaitable;
import cmoon.execution.operation_state;
import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.set_value;
import cmoon.execution.set_done;
import cmoon.execution.set_error;
import cmoon.execution.start;

namespace cmoon::execution
{
	struct op_base
	{
		struct promise_base
		{
			std::suspend_always initial_suspend() noexcept
			{
				return {};
			}

			[[noreturn]] std::suspend_always final_suspend() noexcept
			{
				std::terminate();
			}

			[[noreturn]] void unhandled_exception() noexcept
			{
				std::terminate();
			}

			[[noreturn]] void return_void() noexcept
			{
				std::terminate();
			}

			template<class F>
			auto yield_value(F&& f) noexcept
			{
				struct awaiter
				{
					F f;

					bool await_ready() noexcept
					{
						return false;
					}

					void await_suspend(std::coroutine_handle<>) noexcept(std::is_nothrow_invocable_v<F>)
					{
						std::invoke(std::forward<F>(f));
					}

					[[noreturn]] void await_resume() noexcept
					{
						std::terminate();
					}
				};

				return awaiter{std::forward<F>(f)};
			}
		};

		std::coroutine_handle<> coro;

		explicit op_base(std::coroutine_handle<> coro) noexcept
			: coro{coro} {}

		op_base(op_base&& other) noexcept
			: coro{std::exchange(other.coro, {})} {}

		~op_base()
		{
			if (coro)
			{
				coro.destroy();
			}
		}

		friend void tag_invoke(start_t, op_base& self) noexcept
		{
			self.coro.resume();
		}
	};

	template<class R>
	class op : public op_base
	{
		public:
			struct promise_type : promise_base
			{
				template<class A>
				explicit promise_type(A&, R& r) noexcept
					: r_{r} {}

				std::coroutine_handle<> unhandled_done() noexcept
				{
					set_done(std::move(r_));

					return std::noop_coroutine();
				}

				op get_return_object() noexcept
				{
					return op{std::coroutine_handle<promise_type>::from_promise(*this)};
				}

				template<class... As, std::invocable<R&, As...> CPO>
					requires(!std::same_as<set_value_t, CPO> &&
							 !std::same_as<set_error_t, CPO> &&
							 !std::same_as<set_done_t, CPO>)
				friend auto tag_invoke(CPO cpo, const promise_type& self, As&&... as) noexcept(std::is_nothrow_invocable_v<CPO, R&, As...>)
				{
					return cpo(self.r_, std::forward<As>(as)...);
				}

				R& r_;
			};

		using op_base::op_base;
	};

	inline constexpr struct connect_awaitable_t
	{
		template<is_awaitable A, receiver R>
		auto operator()(A&& a, R&& r) const
		{
			std::exception_ptr ex;
			try
			{
				auto fn = [&](auto&&... as) noexcept
				{
					return [&]() {
						set_value(std::forward<R>(r), std::forward<decltype(as)>(as)...);
					};
				};

				if constexpr (std::is_void_v<await_result_type<A>>)
				{
					co_yield (co_await std::forward<A>(a), std::invoke(fn));
				}
				else
				{
					co_yield std::invoke(fn, co_await std::forward<A>(a));
				}
			}
			catch (...)
			{
				ex = std::current_exception();
			}

			co_yield [&]() noexcept {
				set_error(std::forward<R>(r), std::move(ex));
			};
		}
	} connect_awaitable {};

	export
	struct connect_t
	{
		private:
			enum class state { none, tag_invoke_fn, awaitable_fn };

			template<class S, class R>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(connect_t t, S&& s, R&& r) {
					{ tag_invoke(t, std::forward<S>(s), std::forward<R>(r)) } -> operation_state;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<connect_t, S, R>};
				}
				else if constexpr (is_awaitable<S> && requires(S&& s, R&& r) {
					connect_awaitable(std::forward<S>(s), std::forward<R>(r));
				})
				{
					return {state::awaitable_fn};
				}
				else
				{
					return {state::none};
				}
			}
		public:
			template<sender S, receiver R>
				requires(choose<S, R>().strategy != state::none)
			constexpr decltype(auto) operator()(S&& s, R&& r) const noexcept(choose<S, R>().no_throw)
			{
				constexpr auto choice {choose<S, R>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s), std::forward<R>(r));
				}
				else if constexpr (choice.strategy == state::awaitable_fn)
				{
					return connect_awaitable(std::forward<S>(s), std::forward<R>(r));
				}
			}
	};

	export
	inline constexpr connect_t connect{};

	export
	template<class S, class R>
	using connect_result_t = std::invoke_result_t<decltype(connect), S, R>;
}