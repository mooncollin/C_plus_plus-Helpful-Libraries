export module cmoon.execution.execute;

import <utility>;
import <type_traits>;
import <concepts>;

import cmoon.meta;
import cmoon.type_traits;

import cmoon.execution.impl.executor_impl;
import cmoon.execution.as_receiver;
import cmoon.execution.impl.as_invocable;
import cmoon.execution.receiver;

namespace cmoon::execution
{
	namespace execute_ns
	{
		void execute();

		class execute_t
		{
			private:
				enum class state { none, member_fn, default_fn, submit_fn };

				template<class E, class F>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(const E& e, F&& f) {
						e.execute(std::forward<F>(f));
					})
					{
						return {state::member_fn, noexcept(std::declval<E>().execute(std::declval<F>()))};
					}
					else if constexpr (requires(const E& e, F&& f) {
						execute(e, std::forward<F>(f));
					})
					{
						return {state::default_fn, noexcept(execute(std::declval<E>(), std::declval<F>()))};
					}
					else if constexpr (!cmoon::is_specialization_v<F, as_invocable> &&
									   std::invocable<std::remove_cvref_t<F>&> &&
									   /* sender_to<E, as_receiver<std::remove_cvref_t<F>, E>> */
									   // sender_to requires the definition of execution::execute(), so this will cause circular dependencies.
									   // We must get as close to the definition of sender_to as possible
									   receiver<as_receiver<std::remove_cvref_t<F>, E>>
									   )
					{
						return {state::submit_fn};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<executor_base_concept E, executor_fn_base_concept F>
					requires(choose<E, F>().strategy != state::none)
				constexpr void operator()(const E& e, F&& f) const noexcept(choose<E, F>().no_throw)
				{
					constexpr auto choice {choose<E, F>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						e.execute(std::forward<F>(f));
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						execute(e, std::forward<F>(f));
					}
					else if constexpr (choice.strategy == state::submit_fn)
					{
						submit(e, as_receiver<std::remove_cvref_t<F>, E>{std::forward<F>(f)});
					}
				}
		};
	}

	export
	inline constexpr execute_ns::execute_t execute{};
}