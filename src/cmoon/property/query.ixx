export module cmoon.property.query;

import <utility>;
import <type_traits>;

import cmoon.meta;

import cmoon.property.is_applicable_property;

namespace cmoon
{
	namespace query_cpo
	{
		void query();

		struct cpo
		{
			private:
				enum class state { none, static_query, member_call, non_member_call };

				template<class E, class P>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (cmoon::is_applicable_property_v<std::decay_t<E>, std::decay_t<P>>)
					{
						if constexpr (requires {
							std::decay_t<P>::template static_query_v<std::decay_t<E>>;
						})
						{
							return {state::static_query, true};
						}
						else if constexpr (requires(E&& e, P&& p) {
							std::forward<E>(e).query(std::forward<P>(p));
						})
						{
							return {state::member_call, noexcept((std::declval<E>()).query(std::declval<P>()))};
						}
						else if constexpr (requires(E&& e, P&& p) {
							query(std::forward<E>(e), std::forward<P>(p));
						})
						{
							return {state::non_member_call, noexcept(query(std::declval<E>(), std::declval<P>()))};
						}
						else
						{
							return {state::none};
						}
					}
					else
					{
						return {state::none};
					}
				}

			public:
				template<class E, class P>
					requires(choose<E, P>().strategy != state::none)
				constexpr decltype(auto) operator()(E&& e, P&& p) const noexcept(choose<E, P>().no_throw)
				{
					constexpr auto choice {choose<E, P>()};

					if constexpr (choice.strategy == state::static_query)
					{
						return std::decay_t<P>::template static_query_v<std::decay_t<E>>;
					}
					else if constexpr (choice.strategy == state::member_call)
					{
						return std::forward<E>(e).query(std::forward<P>(p));
					}
					else if constexpr (choice.strategy == state::non_member_call)
					{
						return query(std::forward<E>(e), std::forward<P>(p));
					}
				}
		};
	}

	export
	inline constexpr query_cpo::cpo query {};
}