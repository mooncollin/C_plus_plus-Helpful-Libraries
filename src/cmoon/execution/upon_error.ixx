export module cmoon.execution.upon_error;

import <utility>;
import <exception>;
import <type_traits>;
import <functional>;

import cmoon.functional;
import cmoon.meta;

import cmoon.execution.sender;
import cmoon.execution.sender_base;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.receiver;
import cmoon.execution.get_completion_scheduler;
import cmoon.execution.connect;

namespace cmoon::execution
{
	template<class F, class R>
	struct upon_error_receiver
	{
		public:
			constexpr upon_error_receiver(F&& f, R&& r)
				: f_{std::forward<F>(f)}, out_r{std::forward<R>(r)} {}

			template<class... Args>
			constexpr friend void tag_invoke(set_value_t, upon_error_receiver&& r, Args&&... args) noexcept(cmoon::nothrow_tag_invocable<set_value_t, R, Args...>)
			{
				execution::set_value(std::move(r.out_r), std::forward<Args>(args)...);
			}

			template<class E, class Ret = std::invoke_result_t<F, E>>
				requires(receiver_of<R, Ret>)
			constexpr friend void tag_invoke(set_error_t, upon_error_receiver&& r, E&& e) noexcept
			{
				try
				{
					execution::set_value(std::move(r.out_r), std::invoke(std::move(r.f_), std::forward<E>(e)));
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
			}

			template<class E, class Ret = std::invoke_result_t<F, E>>
				requires(std::is_void_v<Ret> && receiver_of<R>)
			constexpr friend void tag_invoke(set_error_t, upon_error_receiver&& r, E&& e) noexcept
			{
				try
				{
					std::invoke(std::move(r.f_), std::forward<E>(e));
					execution::set_value(std::move(r.out_r));
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
			}

			constexpr friend void tag_invoke(set_done_t, upon_error_receiver&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}
		private:
			F f_;
			R out_r;
	};

	template<class S, class F>
	struct upon_error_sender : public sender_base
	{
		public:
			constexpr upon_error_sender(S&& s, F&& f)
				: s_{std::forward<S>(s)}, f_{std::forward<F>(f)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, upon_error_sender&& s, R&& out_r)
			{
				static_assert(receiver<upon_error_receiver<F, R>>);

				return execution::connect(std::move(s.s_),
										  upon_error_receiver<F, R>{std::move(s.f_), std::forward<R>(out_r)});
			}
		private:
			S s_;
			F f_;
	};

	template<class F>
	struct upon_error_adapter;

	export
	struct upon_error_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S, class F>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(upon_error_t t, S&& s, F&& f) {
					{ tag_invoke(t, get_completion_scheduler<set_error_t>(s), std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_error_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<upon_error_t, cs, S, F>};
				}
				else if constexpr (requires(upon_error_t t, S&& s, F&& f) {
					{ tag_invoke(t, std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<upon_error_t, S, F>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<upon_error_sender<S, F>, S, F>};
				}
			}
		public:
			template<sender S, class F>
			constexpr decltype(auto) operator()(S&& s, F&& f) const noexcept(choose<S, F>().no_throw)
			{
				constexpr auto choice {choose<S, F>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_error_t>(s), std::forward<S>(s), std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s), std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return upon_error_sender<S, F>{std::forward<S>(s), std::forward<F>(f)};
				}
			}

			template<class F>
			constexpr auto operator()(F&& f) const
			{
				return upon_error_adapter<F>{std::forward<F>(f)};
			}
	};

	export
	inline constexpr upon_error_t upon_error{};

	template<class F>
	struct upon_error_adapter
	{
		public:
			constexpr upon_error_adapter(F&& f)
				: f_{std::forward<F>(f)} {}

			template<sender S>
			constexpr friend auto operator|(S&& s, upon_error_adapter&& a)
			{
				return upon_error(std::forward<S>(s), std::move(a.f_));
			}
		private:
			F f_;
	};
}