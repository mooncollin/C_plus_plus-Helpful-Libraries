export module cmoon.execution.let_value;

import <utility>;
import <functional>;
import <exception>;

import cmoon.meta;
import cmoon.functional;

import cmoon.execution.get_completion_scheduler;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.sender;
import cmoon.execution.receiver;
import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.sender_base;
import cmoon.execution.sender_adapter;

namespace cmoon::execution
{
	template<class F, class R>
	struct let_value_receiver
	{
		public:
			constexpr let_value_receiver(F&& f, R&& r)
				: f_{std::forward<F>(f)}, out_r{std::forward<R>(r)} {}

			template<class... Args>
			constexpr friend void tag_invoke(set_value_t, let_value_receiver&& r, Args&&... args) noexcept
			{
				try
				{
					execution::start(
						execution::connect(
							std::invoke(std::move(r.f_), std::forward<Args>(args)...),
							std::move(r.out_r)
						)
					);
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
			}

			template<class E>
			constexpr friend void tag_invoke(set_error_t, let_value_receiver&& r, E&& e) noexcept
			{
				execution::set_error(std::move(r.out_r), std::forward<E>(e));
			}

			constexpr friend void tag_invoke(set_done_t, let_value_receiver&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}
		private:
			F f_;
			R out_r;
	};

	template<class S, class F>
	struct let_value_sender : public sender_base
	{
		public:
			constexpr let_value_sender(S&& s, F&& f)
				: s_{std::forward<S>(s)}, f_{std::forward<F>(f)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, let_value_sender&& s, R&& out_r)
			{
				return execution::connect(std::move(s.s_),
										  let_value_receiver<F, R>{std::move(s.f_), std::forward<R>(out_r)});
			}
		private:
			S s_;
			F f_;
	};

	template<class F>
	struct let_value_adapter;

	export
	struct let_value_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S, class F>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(let_value_t t, S&& s, F&& f) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<let_value_t, cs, S, F>};
				}
				else if constexpr (requires(let_value_t t, S&& s, F&& f) {
					{ tag_invoke(t, std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<let_value_t, S, F>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<let_value_sender<S, F>, S, F>};
				}
			}
		public:
			template<sender S, class F>
			constexpr decltype(auto) operator()(S&& s, F&& f) const noexcept(choose<S, F>().no_throw)
			{
				constexpr auto choice {choose<S, F>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s), std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return let_value_sender<S, F>{std::forward<S>(s), std::forward<F>(f)};
				}
			}

			template<class F>
			constexpr auto operator()(F&& f) const
			{
				return sender_adapter<let_value_t, F>{std::forward<F>(f)};
			}
	};

	export
	inline constexpr let_value_t let_value{};
}