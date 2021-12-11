export module cmoon.execution.let_error;

import <utility>;
import <functional>;
import <exception>;
import <any>;

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
import cmoon.execution.operation_state;

namespace cmoon::execution
{
	template<class F, class R>
	struct let_error_receiver
	{
		public:
			constexpr let_error_receiver(void** o, void(**op_state3_deleter)(void*), F&& f, R&& r)
				: o_{o}, op_state3_deleter{op_state3_deleter}, f_{std::forward<F>(f)}, out_r{std::forward<R>(r)} {}

			template<class... Args>
			friend void tag_invoke(set_value_t, let_error_receiver&& r, Args&&... args)
			{
				execution::set_value(std::move(r.out_r), std::forward<Args>(args)...);
			}

			template<class E>
			friend void tag_invoke(set_error_t, let_error_receiver&& r, E&& e) noexcept
			{
				using c_t = connect_result_t<std::invoke_result_t<F, E>, R>;
				c_t* op;
				try
				{
					op = new c_t{execution::connect(std::invoke(std::move(r.f_), std::forward<E>(e)), std::move(r.out_r))};
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
					return;
				}

				*r.o_ = op;
				*r.op_state3_deleter = deleter<c_t>;
				execution::start(*op);
			}

			friend void tag_invoke(set_done_t, let_error_receiver&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}
		private:
			void** o_;
			void(**op_state3_deleter)(void*);
			F f_;
			R out_r;
	};

	template<sender S, class F, receiver R>
	struct operation
	{
		public:
			operation(S&& s, F&& f, R&& out_r)
				: op_state2{execution::connect(std::forward<S>(s), let_error_receiver<F, R>{std::addressof(op_state3), std::addressof(op_state3_deleter), std::forward<F>(f), std::forward<R>(out_r)})} {}

			friend void tag_invoke(start_t, operation& o) noexcept
			{
				execution::start(o.op_state2);
			}

			~operation() noexcept
			{
				if (op_state3_deleter)
				{
					op_state3_deleter(op_state3);
				}
			}
		private:
			void* op_state3 {nullptr};
			void(*op_state3_deleter)(void*) {nullptr};
			connect_result_t<S, let_error_receiver<F, R>> op_state2;
	};

	template<class S, class F>
	struct let_error_sender : public sender_base
	{
		public:
			constexpr let_error_sender(S&& s, F&& f)
				: s_{std::forward<S>(s)}, f_{std::forward<F>(f)} {}

			template<receiver R>
			friend auto tag_invoke(connect_t, let_error_sender&& s, R&& out_r)
			{
				return operation<std::decay_t<S>, std::decay_t<F>, R>{std::move(s.s_), std::move(s.f_), std::forward<R>(out_r)};
			}
		private:
			S s_;
			F f_;
	};

	template<class F>
	struct let_error_adapter;

	export
	struct let_error_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S, class F>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(let_error_t t, S&& s, F&& f) {
					{ tag_invoke(t, get_completion_scheduler<set_error_t>(s), std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_error_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<let_error_t, cs, S, F>};
				}
				else if constexpr (requires(let_error_t t, S&& s, F&& f) {
					{ tag_invoke(t, std::forward<S>(s), std::forward<F>(f)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<let_error_t, S, F>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<let_error_sender<S, F>, S, F>};
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
					return let_error_sender<S, F>{std::forward<S>(s), std::forward<F>(f)};
				}
			}

			template<class F>
			constexpr auto operator()(F&& f) const
			{
				return sender_adapter<let_error_t, F>{std::forward<F>(f)};
			}
	};

	export
	inline constexpr let_error_t let_error{};
}