#pragma once

#include <utility>

#include "cmoon/meta.hpp"

namespace cmoon
{
	namespace execution
	{
	namespace set_error_cpo
		{
			void set_error();

			template<class R, class E>
			concept has_adl =
				requires(R&& r, E&& e)
			{
				set_error(std::forward<R>(r), std::forward<E>(e));
			};

			template<class R, class E>
			concept can_member_call =
				requires(R&& r, E&& e)
			{
				std::forward<R>(r).set_error(std::forward<E>(e));
			};

			struct cpo
			{
				private:
					enum class state { none, member_call, non_member_call };

					template<class R, class E>
					[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
					{
						if constexpr (can_member_call<R, E>)
						{
							return {state::member_call, noexcept((std::declval<R>()).set_error(std::declval<E>()))};
						}
						else if constexpr (has_adl<R, E>)
						{
							return {state::non_member_call, noexcept(set_error(std::declval<R>(), std::declval<E>()))};
						}
						else
						{
							return {state::none};
						}
					}

					template<class R, class E>
					static constexpr auto choice = choose<R, E>();
				public:
					template<class R, class E>
						requires(choice<R, E>.strategy != state::none)
					constexpr decltype(auto) operator()(R&& r, E&& e) const noexcept(choice<R, E>.no_throw)
					{
						if constexpr (choice<R, E>.strategy == state::member_call)
						{
							return std::forward<R>(r).set_error(std::forward<E>(e));
						}
						else if constexpr (choice<R, E>.strategy == state::non_member_call)
						{
							return set_error(std::forward<R>(r), std::forward<E>(e));
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
			inline constexpr set_error_cpo::cpo set_error {};
		}

		using namespace cpos;
	}
}