export module cmoon.execution.ensure_started;

import <utility>;
import <functional>;
import <atomic>;
import <concepts>;
import <type_traits>;
import <optional>;
import <tuple>;

import cmoon.meta;
import cmoon.functional;

import cmoon.execution.get_completion_scheduler;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.typed_sender;
import cmoon.execution.receiver;
import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.sender_adapter;

namespace cmoon::execution
{
	template<typed_sender S>
	struct state;

	template<typed_sender S>
	struct ensure_started_receiver
	{
		public:
			ensure_started_receiver(state<S>* s)
				: s{s} {}

			template<class... Args>
			constexpr friend void tag_invoke(set_value_t, ensure_started_receiver&& r, Args&&... args)
			{
				r.s->args = std::make_tuple(std::forward<Args>(args)...);
				r.s->operation_done.test_and_set();
				r.s->operation_done.notify_one();
			}

			template<class E>
			constexpr friend void tag_invoke(set_error_t, ensure_started_receiver&& r, E&& e) noexcept
			{
				r.s->err = std::forward<E>(e);
				r.s->operation_done.test_and_set();
				r.s->operation_done.notify_one();
			}

			constexpr friend void tag_invoke(set_done_t, ensure_started_receiver&& r) noexcept
			{
				r.s->operation_done.test_and_set();
				r.s->operation_done.notify_one();
			}
		private:
			state<S>* s;
	};

	template<typed_sender S>
	struct state
	{
		state(S&& s)
			: op_state{execution::connect(std::forward<S>(s), ensure_started_receiver<S>{this})}
		{
			execution::start(op_state);
		}

		connect_result_t<S, ensure_started_receiver<S>> op_state;
		std::optional<value_types_of_t<S>> args;
		std::optional<error_types_of_t<S>> err;
		std::atomic_flag operation_done;
	};

	template<typed_sender S>
	struct ensure_started_sender
	{
		template<receiver R>
		struct op
		{
			public:
				op(R&& r,
				   std::unique_ptr<state<S>> s)
					: out_r{std::forward<R>(r)},
					  s{std::move(s)} {}

				friend void tag_invoke(start_t, op& o) noexcept
				{
					o.s->operation_done.wait(false);

					if (o.s->args)
					{
						std::visit([&o] (auto&& v) {
							std::apply([&o](auto&&... values) {
								execution::set_value(std::move(o.out_r), std::forward<decltype(values)>(values)...);
							},
							std::forward<decltype(v)>(v));
						},
						std::move(o.s->args.value()));
					}
					else if (o.s->err)
					{
						std::visit([&o] (auto&& e) noexcept {
							execution::set_error(std::move(o.out_r), std::forward<decltype(e)>(e));
						},
						std::move(o.s->err.value()));
					}
					else
					{
						execution::set_done(std::move(o.out_r));
					}
				}
			private:
				R out_r;
				std::unique_ptr<state<S>> s;
		};

		public:
			ensure_started_sender(S&& s)
				: s{std::make_unique<state<S>>(std::forward<S>(s))} {}

			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = value_types_of_t<S, Tuple, Variant>;

			template<template<class...> class Variant>
			using error_types = error_types_of_t<S, Variant>;

			static constexpr bool sends_done {true};

			template<receiver R>
			friend auto tag_invoke(connect_t, ensure_started_sender&& s, R&& out_r)
			{
				return op<R>{std::forward<R>(out_r), std::move(s.s)};
			}
		private:
			std::unique_ptr<state<S>> s;
	};

	export
	struct ensure_started_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(ensure_started_t t, S&& s) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s)) } -> sender;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<ensure_started_t, cs, S>};
				}
				else if constexpr (requires(ensure_started_t t, S&& s) {
					{ tag_invoke(t, std::forward<S>(s)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<ensure_started_t, S>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<typed_sender S>
			constexpr decltype(auto) operator()(S&& s) const noexcept(choose<S>().no_throw)
			{
				constexpr auto choice {choose<S>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return ensure_started_sender{std::forward<S>(s)};
				}
			}

			constexpr auto operator()() const noexcept
			{
				return sender_adapter<ensure_started_t>{};
			}
	};

	export
	inline constexpr ensure_started_t ensure_started{};
}