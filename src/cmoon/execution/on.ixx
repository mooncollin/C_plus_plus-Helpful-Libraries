export module cmoon.execution.on;

import <utility>;
import <type_traits>;
import <exception>;

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
import cmoon.execution.sender_base;
import cmoon.execution.receiver;

namespace cmoon::execution
{
	template<class S, class Sch, class R>
	struct on_receiver
	{
		struct on_receiver2
		{
			public:
				constexpr on_receiver2(Sch&& sch, R&& r)
					: sch_{std::forward<Sch>(sch)}, out_r{std::forward<R>(r)} {}

				constexpr auto tag_invoke(get_scheduler_t, on_receiver& r) noexcept
				{
					return r.sch_;
				}

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
			private:
				Sch sch_;
				R out_r;

				friend class on_receiver;
		};

		public:
			constexpr on_receiver(S&& s, Sch&& sch, R&& r)
				: s_{std::forward<S>(s)}, sch_{std::forward<Sch>(sch)}, out_r{std::forward<R>(r)} {}

			constexpr friend void tag_invoke(set_value_t, on_receiver&& r)
			{
				try
				{
					execution::start(
						execution::connect(
							std::move(r.s_),
							on_receiver2{std::move(r.sch_), std::move(r.out_r)}
						)
					);
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
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
			S s_;
			Sch sch_;
			R out_r;
	};

	template<class Sch, class S>
	struct on_sender : public sender_base
	{
		public:
			constexpr on_sender(Sch&& sch, S&& s)
				: sch_{std::forward<Sch>(sch)}, s_{std::forward<S>(s)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, on_sender&& s, R&& out_r)
			{
				return execution::connect(execution::schedule(s.sch_),
										  on_receiver<S, Sch, R>{std::move(s.s_), std::move(s.sch_), std::forward<R>(out_r)});
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