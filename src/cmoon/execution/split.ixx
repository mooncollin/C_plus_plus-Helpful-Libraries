export module cmoon.execution.split;

import <utility>;
import <tuple>;
import <functional>;
import <memory>;
import <mutex>;
import <thread>;
import <optional>;
import <stop_token>;

import cmoon.meta;
import cmoon.functional;
import cmoon.scope;

import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.sender;
import cmoon.execution.typed_sender;
import cmoon.execution.receiver;
import cmoon.execution.get_completion_scheduler;
import cmoon.execution.sender_traits;
import cmoon.execution.connect;
import cmoon.execution.start;

namespace cmoon::execution
{
	template<typed_sender S>
	struct shared_state;

	template<typed_sender S>
	struct split_receiver
	{
		template<class... Args>
		friend void tag_invoke(set_value_t, split_receiver&& r, Args&&... args) noexcept
		{
			r.sh_state->args = std::make_tuple(std::forward<Args>(args)...);
			r.sh_state->received.test_and_set();
			r.sh_state->received.notify_all();
		}

		template<class E>
		friend void tag_invoke(set_error_t, split_receiver&& r, E&& e) noexcept
		{
			r.sh_state->e = std::forward<E>(e);
			r.sh_state->received.test_and_set();
			r.sh_state->received.notify_all();
		}

		friend void tag_invoke(set_done_t, split_receiver&& r) noexcept
		{
			r.sh_state->received.test_and_set();
			r.sh_state->received.notify_all();
		}

		shared_state<S>* sh_state;
	};

	template<typed_sender S>
	struct shared_state
	{
		shared_state(S&& s)
			: op_state2{execution::connect(std::forward<S>(s), split_receiver<S>{this})} {}

		connect_result_t<S, split_receiver<S>> op_state2;
		value_types_of_t<S, std::tuple, std::optional> args;
		error_types_of_t<S, std::optional> e;
		std::atomic_flag received;
		std::once_flag start_once_flag;

		void start() noexcept
		{
			std::call_once(start_once_flag, [this] {
				execution::start(op_state2);
			});
		}
	};

	template<typed_sender S>
	struct split_sender
	{
		private:
			template<receiver R>
			struct op
			{
				public:
					op(std::shared_ptr<shared_state<S>> s, R&& r)
						: sh_state{std::move(s)}, t{&op::wait_on_receiver, std::ref(*this), stop_source.get_token(), sh_state, std::forward<R>(r)} {}

					friend void tag_invoke(start_t, op& o) noexcept
					{
						o.sh_state->start();
						o.started.test_and_set();
						o.started.notify_one();
					}

					~op() noexcept
					{
						if (!started.test())
						{
							stop_source.request_stop();
							started.test_and_set();
							started.notify_one();
							t.join();
						}
						else
						{
							t.detach();
						}
					}
				private:
					std::atomic_flag started;
					std::stop_source stop_source;
					std::shared_ptr<shared_state<S>> sh_state;
					std::thread t;

					static void wait_on_receiver(op& o, std::stop_token token, std::shared_ptr<shared_state<S>> sh_state, R&& r)
					{
						o.started.wait(false);
						if (token.stop_requested())
						{
							return;
						}

						sh_state->received.wait(false);

						if (sh_state->args)
						{
							std::apply(
								[&r] (auto&&... args) {
									execution::set_value(std::forward<R>(r), std::forward<decltype(args)>(args)...);
								},
							sh_state->args.value());
						}
						else if (sh_state->e)
						{
							execution::set_error(std::forward<R>(r), sh_state->e.value());
						}
						else
						{
							execution::set_done(std::forward<R>(r));
						}
					}
			};
		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = typename sender_traits<S>::template value_types<Tuple, Variant>;

			template<template<class...> class Variant>
			using error_types = typename sender_traits<S>::template error_types<Variant>;

			static constexpr bool sends_done {S::sends_done};

			split_sender(S&& s)
				: sh_state{std::make_shared<shared_state<S>>(std::forward<S>(s))} {}

			template<receiver R>
			friend auto tag_invoke(connect_t, split_sender& s, R&& out_r)
			{
				return op<R>{s.sh_state, std::forward<R>(out_r)};
			}

			template<receiver R>
			friend auto tag_invoke(connect_t, split_sender&& s, R&& out_r)
			{
				return op<R>{std::move(s.sh_state), std::forward<R>(out_r)};
			}
		private:
			std::shared_ptr<shared_state<S>> sh_state;
	};

	export
	struct split_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(split_t t, S&& s) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s)) } -> sender;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<split_t, cs, S>};
				}
				else if constexpr (requires(split_t t, S&& s) {
					{ tag_invoke(t, std::forward<S>(s)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<split_t, S>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<split_sender<S>, S>};
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
					return split_sender<S>{std::forward<S>(s)};
				}
			}
	};

	export
	inline constexpr split_t split{};
}