export module cmoon.property.prefer;

import <utility>;
import <type_traits>;

import cmoon.meta;

import cmoon.property.is_applicable_property;

namespace cmoon
{
	namespace prefer_cpo
	{
		struct cpo;

		template <class T, class P>
		void prefer(T&, const P&) = delete;

		template <class T, class P>
		void prefer(const T&, const P&) = delete;

		template<class E, class P0>
		concept has_adl = 
			requires(E&& e, P0&& p0)
		{
			prefer(std::forward<E>(e), std::forward<P0>(p0));
		};

		template<class E, class P0>
		concept applicable = 
			requires
		{
			cmoon::is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>>;
			std::decay_t<P0>::is_preferable;
		} && 
			cmoon::is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>> && 
			std::decay_t<P0>::is_preferable;

		template<class E, class P0>
		concept can_static_query = 
			requires
		{
			std::decay_t<P0>::template static_query_v<std::decay_t<E>>;
			std::decay_t<P0>::value();
		} &&
			std::decay_t<P0>::template static_query_v<std::decay_t<E>> == std::decay_t<P0>::value();

		template<class E, class P0>
		concept can_member_call = 
			requires(E&& e, P0&& p0)
		{
			std::forward<E>(e).prefer(std::forward<P0>(p0));
		};

		template<class E, class P0, class... Pn>
		concept can_chain_require =
			requires(E&& e, P0&& p0, Pn&&... pn)
		{
			cpo{}(cpo{}(std::forward<E>(e), std::forward<P0>(p0)), std::forward<Pn>(pn)...);
		};

		struct cpo
		{
			private:
				enum class state { none, static_query, member_call, non_member_call, chain_call };

				template<class E, class P0, class... Pn>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					constexpr auto N = sizeof...(Pn);

					if constexpr (applicable<E, P0>)
					{
						if constexpr (can_static_query<E, P0> && N == 0)
						{
							return {state::static_query, noexcept(std::declval<E>())};
						}
						else if constexpr (can_member_call<E, P0> && N == 0)
						{
							return {state::member_call, noexcept((std::declval<E>()).prefer(std::declval<P0>()))};
						}
						else if constexpr (has_adl<E, P0> && N == 0)
						{
							return {state::non_member_call, noexcept(prefer(std::declval<E>(), std::declval<P0>()))};
						}
						else if constexpr (can_chain_require<E, P0, Pn...> && N > 0)
						{
							return {state::chain_call, noexcept(cpo{}(cpo{}(std::declval<E>(), std::declval<P0>()), std::declval<Pn>()...))};
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

				template<class E, class P0, class... Pn>
				static constexpr auto choice = choose<E, P0, Pn...>();
			public:
				template<class E, class P0, class... Pn>
					requires(choice<E, P0, Pn...>.strategy != state::none)
				constexpr decltype(auto) operator()(E&& e, P0&& p0, Pn&&... pn) const noexcept(choice<E, P0, Pn...>.no_throw)
				{
					if constexpr (choice<E, P0, Pn...>.strategy == state::static_query)
					{
						return std::forward<E>(e);
					}
					else if constexpr (choice<E, P0, Pn...>.strategy == state::member_call)
					{
						return std::forward<E>(e).prefer(std::forward<P0>(p0));
					}
					else if constexpr (choice<E, P0, Pn...>.strategy == state::non_member_call)
					{
						return prefer(std::forward<E>(e), std::forward<P0>(p0));
					}
					else if constexpr (choice<E, P0, Pn...>.strategy == state::chain_call)
					{
						return (*this)((*this)(std::forward<E>(e), std::forward<P0>(p0)), std::forward<Pn>(pn)...);
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
		export
		inline constexpr prefer_cpo::cpo prefer {};
	}

	using namespace cpos;
}