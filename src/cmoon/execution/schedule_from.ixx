export module cmoon.execution.schedule_from;

import <utility>;
import <concepts>;
import <exception>;

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
import cmoon.execution.sender_base;

namespace cmoon::execution
{
	template<class Sch, class R>
	struct schedule_from_receiver
	{
		public:
			struct schedule_from_receiver2
			{
				public:
					constexpr schedule_from_receiver2(R&& r)
						: out_r{std::forward<R>(r)} {}

					template<class... Args>
					constexpr friend void tag_invoke(set_value_t, schedule_from_receiver2&& r, Args&&... args)
					{
						execution::set_value(std::move(r.out_r), std::forward<Args>(args)...);
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

					friend struct schedule_from_receiver;
			};

			constexpr schedule_from_receiver(Sch&& sch, R&& r)
				: sch_{std::forward<Sch>(sch)}, out_r{std::forward<R>(r)} {}

			template<class CPO, class... Args>
				requires(std::same_as<std::remove_cvref_t<CPO>, set_value_t> ||
						 std::same_as<std::remove_cvref_t<CPO>, set_error_t> ||
						 std::same_as<std::remove_cvref_t<CPO>, set_done_t>)
			constexpr friend void tag_invoke(CPO, schedule_from_receiver&& r, Args&&... args) noexcept
			{
				try
				{
					execution::start(
						execution::connect(
							execution::schedule(std::move(r.sch_)),
							schedule_from_receiver2{std::move(r.out_r)}
						)
					);
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
			}
		private:
			Sch sch_;
			R out_r;
	};

	template<class Sch, class S>
	struct schedule_from_sender : public sender_base
	{
		public:
			constexpr schedule_from_sender(Sch&& sch, S&& s)
				: sch_{std::forward<Sch>(sch)}, s_{std::forward<S>(s)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, schedule_from_sender&& s, R&& out_r)
			{
				return execution::connect(std::move(s.s_),
										  schedule_from_receiver<Sch, R>{std::move(s.sch_), std::forward<R>(out_r)});
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
					return schedule_from_sender<Sch, S>{std::forward<Sch>(sch), std::forward<S>(s)};
				}
			}
	};

	export
	inline constexpr schedule_from_t schedule_from{};
}