#pragma once

#include <utility>

#include "cmoon/meta.hpp"
#include "cmoon/execution/impl/submit.hpp"
#include "cmoon/execution/impl/as_receiver.hpp"
#include "cmoon/execution/impl/submit.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace execute_cpo
		{
			void execute();

			template<class E, class F>
			concept has_adl =
				requires(E&& e, F&& f)
			{
				execute(std::forward<E>(e), std::forward<F>(f));
			};

			template<class E, class F>
			concept can_member_call =
				requires(E&& e, F&& f)
			{
				std::forward<E>(e).execute(std::forward<F>(f));
			};

			template<class E, class F>
			concept can_submit = std::invocable<std::remove_cvref_t<F>&> &&
				requires(E&& e, F&& f)
			{
				execution::submit(std::forward<E>(e), details::as_receiver<std::remove_cvref_t<F>, E>{std::forward<F>(f)});
			};

			struct cpo
			{
				private:
					enum class state { none, member_call, non_member_call, submit_call };

					template<class E, class F>
					[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
					{
						if constexpr (can_member_call<E, F>)
						{
							return {state::member_call, noexcept(std::declval<E>().execute(std::declval<F>()))};
						}
						else if constexpr (has_adl<E, F>)
						{
							return {state::non_member_call, noexcept(execute(std::declval<E>(), std::declval<F>()))};
						}
						else if constexpr (can_submit<E, F>)
						{
							return {state::submit_call, noexcept(execution::submit(std::declval<E>(), details::as_receiver<std::remove_cvref_t<F>, E>{std::declval<F>()}))};
						}
					}

					template<class E, class F>
					static constexpr auto choice = choose<E, F>();
				public:
					template<class E, class F>
						requires(choice<E, F>.strategy != state::none)
					constexpr decltype(auto) operator()(E&& e, F&& f) const noexcept(choice<E, F>.no_throw)
					{
						if constexpr (choice<E, F>.strategy == state::member_call)
						{
							return std::forward<E>(e).execute(std::forward<F>(f));
						}
						else if constexpr (choice<E, F>.strategy == state::non_member_call)
						{
							return execute(std::forward<E>(e), std::forward<F>(f));
						}
						else if constexpr (choice<E, F>.strategy == state::submit_call)
						{
							return execution::submit(std::forward<E>(e), details::as_receiver<std::remove_cvref_t<F>, E>{std::forward<F>(f)});
						}
						else
						{
							static_assert(false, "should be unreachable");
						}
					}
			};
		}

		namespace cpos
		{
			inline constexpr execute_cpo::cpo execute {};
		}

		using namespace cpos;
	}
}