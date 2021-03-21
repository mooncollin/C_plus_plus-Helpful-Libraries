#pragma once

#include <utility>

#include "cmoon/meta.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace start_cpo
		{
			void start();

			template<class O>
			concept has_adl =
				requires(O&& o)
			{
				start(std::forward<O>(o));
			};

			template<class O>
			concept can_member_call =
				requires(O&& o)
			{
				std::forward<O>(o).start();
			};

			struct cpo
			{
				private:
					enum class state { none, member_call, non_member_call };

					template<class O>
					[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
					{
						if constexpr (can_member_call<O>)
						{
							return {state::member_call, noexcept((std::declval<O>()).start())};
						}
						else if constexpr (has_adl<O>)
						{
							return {state::non_member_call, noexcept(start(std::declval<O>()))};
						}
						else
						{
							return {state::none};
						}
					}

					template<class O>
					static constexpr auto choice = choose<O>();
				public:
					template<class O>
						requires(choice<O>.strategy != state::none)
					void operator()(O&& o) const noexcept(choice<O>.no_throw)
					{
						if constexpr (choice<O>.strategy == state::member_call)
						{
							return std::forward<O>(o).start();
						}
						else if constexpr (choice<O>.strategy == state::non_member_call)
						{
							return start(std::forward<O>(o));
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
			inline constexpr start_cpo::cpo start {};
		}

		using namespace cpos;
	}
}