export module cmoon.execution.start_detached;

import <utility>;
import <functional>;
import <atomic>;
import <concepts>;
import <type_traits>;

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

namespace cmoon::execution
{
	template<sender S>
	struct start_detached_impl
	{
		struct wrap_r
		{
			template<class... Args>
			constexpr friend void tag_invoke(set_value_t, start_detached_impl::wrap_r&& r, Args&&...) noexcept
			{
				delete r.impl;
			}

			template<class E>
			constexpr friend void tag_invoke(set_error_t, start_detached_impl::wrap_r&&, E&&) noexcept
			{
				std::terminate();
			}

			constexpr friend void tag_invoke(set_done_t, start_detached_impl::wrap_r&& r) noexcept
			{
				delete r.impl;
			}

			start_detached_impl* impl;
		};

		connect_result_t<S, wrap_r> state;

		start_detached_impl(S&& s) noexcept(cmoon::nothrow_tag_invocable<connect_t, S, wrap_r>)
			: state{execution::connect(std::forward<S>(s), wrap_r{this})} {}
	};

	export
	struct start_detached_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(start_detached_t t, S&& s) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s)) } -> std::same_as<void>;
				})
				{
					using cs = std::invoke_result_t<get_completion_scheduler_t<set_value_t>, S>;
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<start_detached_t, cs, S>};
				}
				else if constexpr (requires(start_detached_t t, S&& s) {
					{ tag_invoke(t, std::forward<S>(s)) } -> std::same_as<void>;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<start_detached_t, S>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<sender S>
			constexpr void operator()(S&& s) const noexcept(choose<S>().no_throw)
			{
				constexpr auto choice {choose<S>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					tag_invoke(*this, std::forward<S>(s));
				}
				else if constexpr (choice.strategy == state::other)
				{
					auto impl {new start_detached_impl<std::decay_t<S>>{std::forward<S>(s)}};
					execution::start(impl->state);
				}
			}
	};

	export
	inline constexpr start_detached_t start_detached{};
}