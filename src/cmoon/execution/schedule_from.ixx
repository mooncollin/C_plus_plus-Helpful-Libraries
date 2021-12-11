export module cmoon.execution.schedule_from;

import <utility>;
import <concepts>;
import <exception>;
import <optional>;
import <tuple>;
import <variant>;

import cmoon.functional;
import cmoon.meta;

import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;
import cmoon.execution.schedule;
import cmoon.execution.scheduler;
import cmoon.execution.sender;
import cmoon.execution.receiver;
import cmoon.execution.typed_sender;
import cmoon.execution.sender_traits;
import cmoon.execution.receiver;
import cmoon.execution.sender_base;

namespace cmoon::execution
{
	template<class P>
	void deleter(void* p)
	{
		delete static_cast<P*>(p);
	}

	template<class CPO, receiver R, class... Args>
	struct schedule_from_receiver2
	{
		public:
			constexpr schedule_from_receiver2(R&& r, Args&&... args)
				: out_r{std::forward<R>(r)}, args{std::forward<Args>(args)...} {}

			constexpr friend void tag_invoke(set_value_t, schedule_from_receiver2&& r)
			{
				std::apply(
					[&r](auto&&... args) {
						std::invoke(CPO{}, std::forward<decltype(args)>(args)...);
					},
				std::move(r.args));
			}

			template<class E>
			constexpr friend void tag_invoke(set_error_t, schedule_from_receiver2&& r, E&& e) noexcept
			{
				execution::set_error(std::move(r.out_r), std::forward<E>(e));
			}

			constexpr friend void tag_invoke(set_done_t, schedule_from_receiver2&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}
		private:
			R out_r;
			std::tuple<Args...> args;
	};

	template<scheduler Sch, typed_sender S, receiver R>
	struct operation
	{
		public:
			struct schedule_from_receiver
			{
				public:
					constexpr schedule_from_receiver(operation* op, Sch&& sch, R&& r)
						: op_{op}, sch_{std::forward<Sch>(sch)}, out_r{std::forward<R>(r)} {}

					template<class CPO, class... Args>
						requires(std::same_as<std::remove_cvref_t<CPO>, set_value_t> ||
								 std::same_as<std::remove_cvref_t<CPO>, set_error_t> ||
								 std::same_as<std::remove_cvref_t<CPO>, set_done_t>)
					constexpr friend void tag_invoke(CPO, schedule_from_receiver&& r, Args&&... args) noexcept
					{
						r.tag_invoke_helper<CPO>(std::forward<Args>(args)...);
					}
				private:
					operation* op_;
					Sch sch_;
					R out_r;

					template<class CPO, class... Args>
					constexpr void tag_invoke_helper(Args&&... args) noexcept
					{
						using c_t = connect_result_t<schedule_result_t<Sch>, schedule_from_receiver2<CPO, R, Args...>>;
						c_t* op;
						try
						{
							op = new c_t{execution::connect(execution::schedule(sch_),
										 schedule_from_receiver2<CPO>{std::move(out_r), std::forward<Args>(args)...})};
						}
						catch (...)
						{
							execution::set_error(std::move(out_r), std::current_exception());
							return;
						}

						op_->op_state3 = op;
						op_->op_state3_deleter = deleter<c_t>;
						execution::start(*op);
					}
			};

			friend struct schedule_from_receiver;

			constexpr operation(Sch&& sch, S&& s, R&& out_r)
				: op_state2{execution::connect(std::forward<S>(s), schedule_from_receiver{this, std::forward<Sch>(sch), std::forward<R>(out_r)})} {}

			constexpr friend void tag_invoke(start_t, operation& o) noexcept
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
			connect_result_t<S, schedule_from_receiver> op_state2;
			void* op_state3 {nullptr};
			void(*op_state3_deleter)(void*) {nullptr};

	};

	template<scheduler Sch, typed_sender S>
	struct schedule_from_sender
	{
		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = value_types_of_t<S, Tuple, Variant>;

			template<template<class...> class Variant>
			using error_types = error_types_of_t<S, Variant>;

			static constexpr bool sends_done {sender_traits<S>::sends_done};

			constexpr schedule_from_sender(Sch&& sch, S&& s)
				: sch_{std::forward<Sch>(sch)}, s_{std::forward<S>(s)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, schedule_from_sender&& s, R&& out_r)
			{
				return operation<Sch, S, R>{std::move(s.sch_), std::move(s.s_), std::forward<R>(out_r)};
			}
		private:
			Sch sch_;
			S s_;
	};

	export
	struct schedule_from_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class Sch, class S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(schedule_from_t t, Sch&& sch, S&& s) {
					{ tag_invoke(t, std::forward<Sch>(sch), std::forward<S>(s)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<schedule_from_t, Sch, S>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<scheduler Sch, typed_sender S>
			constexpr decltype(auto) operator()(Sch&& sch, S&& s) const noexcept(choose<Sch, S>().no_throw)
			{
				constexpr auto choice {choose<Sch, S>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<Sch>(sch), std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return schedule_from_sender<Sch, S>{std::forward<Sch>(sch), std::forward<S>(s)};
				}
			}
	};

	export
	inline constexpr schedule_from_t schedule_from{};
}