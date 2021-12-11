export module cmoon.execution.bulk;

import <utility>;
import <functional>;
import <exception>;
import <concepts>;

import cmoon.meta;
import cmoon.concepts;
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
	template<class Shape, class F, class R>
	struct bulk_receiver
	{
		public:
			constexpr bulk_receiver(Shape shape, F&& f, R&& r)
				: shape{shape}, f_{std::forward<F>(f)}, out_r{std::forward<R>(r)} {}

			template<class... Args>
			constexpr friend void tag_invoke(set_value_t, bulk_receiver&& r, Args&&... args) noexcept
			{
				try
				{
					for (Shape i {0}; i < r.shape; ++i)
					{
						std::invoke(r.f_, i, args...);
					}

					execution::set_value(std::move(r.out_r), args...);
				}
				catch (...)
				{
					execution::set_error(std::move(r.out_r), std::current_exception());
				}
			}

			template<class E>
			constexpr friend void tag_invoke(set_error_t, bulk_receiver&& r, E&& e) noexcept
			{
				execution::set_error(std::move(r.out_r), std::forward<E>(e));
			}

			constexpr friend void tag_invoke(set_done_t, bulk_receiver&& r) noexcept
			{
				execution::set_done(std::move(r.out_r));
			}
		private:
			Shape shape;
			F f_;
			R out_r;
	};

	template<class S, class Shape, class F>
	struct bulk_sender : public sender_base
	{
		public:
			constexpr bulk_sender(S&& s, Shape shape, F&& f)
				: s_{std::forward<S>(s)}, shape{shape}, f_{std::forward<F>(f)} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, bulk_sender&& s, R&& out_r)
			{
				return execution::connect(std::move(s.s_),
										  bulk_receiver<Shape, F, R>{s.shape, std::move(s.f_), std::forward<R>(out_r)});
			}
		private:
			S s_;
			Shape shape;
			F f_;
	};

	export
	struct bulk_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S, class Shape, class F>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(bulk_t t, S&& s, Shape shape, F&& f) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), shape, std::forward<F>(f)) } -> sender;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<bulk_t, cs, S, F>};
				}
				else if constexpr (requires(bulk_t t, S&& s, Shape shape, F&& f) {
					{ tag_invoke(t, std::forward<S>(s), shape, std::forward<F>(f)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<bulk_t, S, F>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<bulk_sender<S, Shape, F>, S, Shape, F>};
				}
			}
		public:
			template<sender S, std::integral Shape, class F>
			constexpr decltype(auto) operator()(S&& s, Shape shape, F&& f) const noexcept(choose<S, Shape, F>().no_throw)
			{
				constexpr auto choice {choose<S, Shape, F>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), shape, std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s), shape, std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return bulk_sender<S, Shape, F>{std::forward<S>(s), shape, std::forward<F>(f)};
				}
			}

			template<class F, std::integral Shape>
			constexpr auto operator()(F&& f, Shape shape) const
			{
				return sender_adapter<bulk_t, Shape, F>{std::move(shape), std::forward<F>(f)};
			}
	};

	export
	inline constexpr bulk_t bulk{};
}