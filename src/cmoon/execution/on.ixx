export module cmoon.execution.on;

import <utility>;
import <type_traits>;
import <exception>;
import <optional>;

import cmoon.functional;
import cmoon.meta;

import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.schedule;
import cmoon.execution.scheduler;
import cmoon.execution.get_scheduler;
import cmoon.execution.sender;
import cmoon.execution.typed_sender;
import cmoon.execution.sender_base;
import cmoon.execution.sender_traits;
import cmoon.execution.receiver;

namespace cmoon::execution
{
	template<scheduler Sch, receiver R>
	struct on_receiver2
	{
		public:
			constexpr on_receiver2(Sch&& sch, R&& r)
				: sch_{std::forward<Sch>(sch)}, out_r{std::forward<R>(r)} {}

			template<class... Args>
			constexpr friend void tag_invoke(set_value_t, on_receiver2&& r, Args&&... args)
			{
				execution::set_value(std::move(r.out_r), std::forward<Args>(args)...);
			}

			template<class E>
			constexpr friend void tag_invoke(set_error_t, on_receiver2&& r, E&& e) noexcept
			{
				execution::set_error(std::move(r.out_r), std::forward<E>(e));
			}

			constexpr friend void tag_invoke(set_done_t, on_receiver2&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}

			constexpr friend auto tag_invoke(get_scheduler_t, on_receiver2& r) noexcept
			{
				return r.sch_;
			}
		private:
			Sch sch_;
			R out_r;
	};

	template<class Sch, class S, class R>
	struct operation
	{
		struct on_receiver
		{
			public:
				constexpr on_receiver(operation* op_state1, Sch&& sch, S&& s, R&& out_r)
					: op_state1{op_state1},
					  sch_{std::forward<Sch>(sch)},
					  s_{std::forward<S>(s)},
					  out_r{std::forward<R>(out_r)} {}

				constexpr friend void tag_invoke(set_value_t, on_receiver&& r) noexcept
				{
					r.set_value_helper();
				}

				template<class E>
				constexpr friend void tag_invoke(set_error_t, on_receiver&& r, E&& e) noexcept
				{
					execution::set_error(std::move(r.out_r), std::forward<E>(e));
				}

				constexpr friend void tag_invoke(set_done_t, on_receiver&& r) noexcept
				{
					execution::set_done(std::move(r.out_r));
				}
			private:
				operation* op_state1;
				Sch sch_;
				S s_;
				R out_r;

				inline void set_value_helper() noexcept
				{
					try
					{
						op_state1->op_state3.emplace(execution::connect(std::move(s_),
													 on_receiver2{std::move(sch_), std::move(out_r)}));
					}
					catch (...)
					{
						execution::set_error(std::move(out_r), std::current_exception());
					}

					op_state1->op_state2.reset();
					execution::start(op_state1->op_state3.value());
				}
		};

		friend struct on_receiver;

		constexpr operation(Sch&& sch, S&& s, R&& out_r)
			:	op_state2{execution::connect(execution::schedule(sch),
						  on_receiver{this, std::forward<Sch>(sch), std::forward<S>(s), std::forward<R>(out_r)})} {}

		constexpr friend void tag_invoke(start_t, operation& op) noexcept
		{
			execution::start(op.op_state2.value());
		}
		private:
			std::optional<connect_result_t<schedule_result_t<Sch>, on_receiver>> op_state2;
			std::optional<connect_result_t<S, on_receiver2<Sch, R>>> op_state3;
	};

	template<sender S>
	struct on_sender_base : public sender_base {};

	template<typed_sender S>
	struct on_sender_base<S>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = value_types_of_t<S, Tuple, Variant>;

		template<template<class...> class Variant>
		using error_types = error_types_of_t<S, Variant>;

		static constexpr bool sends_done{sender_traits<S>::sends_done};
	};


	template<scheduler Sch, sender S>
	struct on_sender : public on_sender_base<S>
	{
		public:
			constexpr on_sender(Sch&& sch, S&& s)
				: sch_{std::forward<Sch>(sch)}, s_{std::forward<S>(s)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, on_sender&& s, R&& out_r)
			{
				return operation<Sch, S, R>{std::move(s.sch_), std::move(s.s_), std::forward<R>(out_r)};
			}
		private:
			Sch sch_;
			S s_;
	};

	template<scheduler Sch>
	struct on_adapter
	{
		public:
			constexpr on_adapter(Sch&& sch)
				: sch_{std::forward<Sch>(sch)} {}

			template<sender S>
			constexpr friend auto operator|(S&& s, on_adapter&& a)
			{
				return on(std::move(a.sch_), std::forward<S>(s));
			}
		private:
			Sch sch_;
	};

	export
	struct on_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class Sch, class S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(on_t t, Sch&& sch, S&& s) {
					{ tag_invoke(t, std::forward<Sch>(sch), std::forward<S>(s)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<on_t, Sch, S>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<on_sender<Sch, S>, Sch, S>};
				}
			}
		public:
			template<scheduler Sch, sender S>
			constexpr decltype(auto) operator()(Sch&& sch, S&& s) const noexcept(choose<Sch, S>().no_throw)
			{
				constexpr auto choice {choose<Sch, S>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<Sch>(sch), std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return on_sender<Sch, S>{std::forward<Sch>(sch), std::forward<S>(s)};
				}
			}

			template<scheduler Sch>
			constexpr auto operator()(Sch&& sch) const
			{
				return on_adapter<Sch>{std::forward<Sch>(sch)};
			}
	};

	export
	inline constexpr on_t on{};
}