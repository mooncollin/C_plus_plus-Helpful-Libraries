export module cmoon.property.require;

import <utility>;
import <type_traits>;

import cmoon.meta;

import cmoon.property.is_applicable_property;

namespace cmoon
{
	namespace require_cpo
	{
		void require();

		template<class E, class P0>
		concept has_adl = 
			requires(E&& e, P0&& p0)
		{
			require(std::forward<E>(e), std::forward<P0>(p0));
		};

		template<class E, class P0>
		concept applicable = requires 
		{
			std::decay_t<P0>::is_requirable;
		} &&
			cmoon::is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>> &&
			std::decay_t<P0>::is_requirable;

		template<class E, class P0>
		concept can_static_query = requires
		{
			std::decay_t<P0>::template static_query_v<std::decay_t<E>>;
			std::decay_t<P0>::value();
		} &&
			std::decay_t<P0>::template static_query_v<std::decay_t<E>> == std::decay_t<P0>::value();

		template<class E, class P0>
		concept can_member_call = 
			requires(E&& e, P0&& p0)
		{
			std::forward<E>(e).require(std::forward<P0>(p0));
		};

		struct cpo
		{
			private:
				enum class state {none, static_query, member_call, non_member_call, chain_call};

				template<class E, class P0, class... Pn>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					constexpr auto N = sizeof...(Pn);

					if constexpr (applicable<E, P0>)
					{
						if constexpr (can_static_query<E, P0> && N == 0)
						{
							return {state::static_query, std::is_nothrow_default_constructible_v<E>};
						}
						else if constexpr (can_member_call<E, P0> && N == 0)
						{
							return {state::member_call, noexcept((std::declval<E>()).require(std::declval<P0>()))};
						}
						else if constexpr (has_adl<E, P0> && N == 0)
						{
							return {state::non_member_call, noexcept(require(std::declval<E>(), std::declval<P0>()))};
						}
						else if constexpr (N > 0)
						{
							return {state::chain_call};
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
				template<class E, class P0, class... Pn>
					requires(choose<E, P0, Pn...>().strategy != state::none)
				constexpr auto operator()(E&& e, P0&& p0, Pn&&... pn) const noexcept(choose<E, P0, Pn...>().no_throw)
				{
					constexpr auto choice {choose<E, P0, Pn...>()};

					if constexpr (choice.strategy == state::static_query)
					{
						return std::forward<E>(e);
					}
					else if constexpr (choice.strategy == state::member_call)
					{
						return std::forward<E>(e).require(std::forward<P0>(p0));
					}
					else if constexpr (choice.strategy == state::non_member_call)
					{
						return require(std::forward<E>(e), std::forward<P0>(p0));
					}
					else if constexpr (choice.strategy == state::chain_call)
					{
						return (*this)((*this)(std::forward<E>(e), std::forward<P0>(p0)), std::forward<Pn>(pn)...);
					}
				}
		};
	}

	export
	inline constexpr require_cpo::cpo require {};
}