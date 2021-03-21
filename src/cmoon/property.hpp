#pragma once

#include <type_traits>
#include <ranges>

#include "cmoon/concepts.hpp"
#include "cmoon/meta.hpp"

#include <ranges>

namespace cmoon
{
	template<class T, class P>
	struct is_applicable_property : std::false_type {};

	template<cmoon::complete_type T, cmoon::complete_type P>
		requires(requires { P::template is_applicable_property_v<T>; } && P::template is_applicable_property_v<T>)
	struct is_applicable_property<T, P> : std::true_type {};

	template<class T, class P>
	inline constexpr bool is_applicable_property_v = is_applicable_property<T, P>::value;

	namespace require_concept_cpo
	{
		void require_concept();

		template<class E, class P>
		concept has_adl = 
			requires(E&& e, P&& p)
		{
			require_concept(std::forward<E>(e), std::forward<P>(p));
		};

		template<class E, class P>
		concept applicable = 
			requires
		{
			is_applicable_property_v<std::decay_t<E>, P>;
			P::is_requirable_concept;
		} && 
			is_applicable_property_v<std::decay_t<E>, P> && 
			P::is_requirable_concept;

		template<class E, class P>
		concept can_static_query = 
			requires
		{
			P::template static_query_v<std::decay_t<E>>;
			P::value();
		} &&
			P::template static_query_v<std::decay_t<E>> == P::value();

		template<class E, class P>
		concept can_member_call = 
			requires(E&& e, P&& p)
		{
			std::forward<E>(e).require_concept(std::forward<P>(p));
		};

		struct cpo
		{
			private:
				enum class state { none, static_query, member_call, non_member_call };

				template<class E, class P>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (applicable<E, P>)
					{
						if constexpr (can_static_query<E, P>)
						{
							return {state::static_query, noexcept(std::declval<E>())};
						}
						else if constexpr (can_member_call<E, P>)
						{
							return {state::member_call, noexcept((std::declval<E>()).require_concept(std::declval<P>()))};
						}
						else if constexpr (has_adl<E, P>)
						{
							return {state::non_member_call, noexcept(require_concept(std::declval<E>(), std::declval<P>()))};
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

				template<class E, class P>
				static constexpr auto choice = choose<E, P>();

			public:
				template<class E, class P>
					requires(choice<E, P>.strategy != state::none)
				constexpr decltype(auto) operator()(E&& e, P&& p) const noexcept(choice<E, P>.no_throw)
				{
					if constexpr (choice<E, P>.strategy == state::static_query)
					{
						return std::forward<E>(e);
					}
					else if constexpr (choice<E, P>.strategy == state::member_call)
					{
						return std::forward<E>(e).require_concept(std::forward<P>(p));
					}
					else if constexpr (choice<E, P>.strategy == state::non_member_call)
					{
						return require_concept(std::forward<E>(e), std::forward<P>(p));
					}
					else
					{
						static_assert(false, "should be unreachable");
					}
				}
		};
	}

	namespace require_cpo
	{
		struct cpo;

		template <class T, class P>
		void require(T&, const P&) = delete;

		template <class T, class P>
		void require(const T&, const P&) = delete;

		template<class E, class P0>
		concept has_adl = 
			requires(E&& e, P0&& p0)
		{
			require(std::forward<E>(e), std::forward<P0>(p0));
		};

		template<class E, class P0>
		concept applicable = 
			requires
		{
			is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>>;
			std::decay_t<P0>::is_requirable;
		} && 
			is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>> && 
			std::decay_t<P0>::is_requirable;

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
			std::forward<E>(e).require(std::forward<P0>(p0));
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
							return {state::member_call, noexcept((std::declval<E>()).require(std::declval<P0>()))};
						}
						else if constexpr (has_adl<E, P0> && N == 0)
						{
							return {state::non_member_call, noexcept(require(std::declval<E>(), std::declval<P0>()))};
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
						return std::forward<E>(e).require(std::forward<P0>(p0));
					}
					else if constexpr (choice<E, P0, Pn...>.strategy == state::non_member_call)
					{
						return require(std::forward<E>(e), std::forward<P0>(p0));
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
			is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>>;
			std::decay_t<P0>::is_preferable;
		} && 
			is_applicable_property_v<std::decay_t<E>, std::decay_t<P0>> && 
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

	namespace query_cpo
	{
		void query();

		template<class E, class P>
		concept has_adl = 
			requires(E&& e, P&& p)
		{
			query(std::forward<E>(e), std::forward<P>(p));
		};

		template<class E, class P>
		concept applicable = 
			requires
		{
			is_applicable_property_v<std::decay_t<E>, std::decay_t<P>>;
		} && 
			is_applicable_property_v<std::decay_t<E>, std::decay_t<P>>;

		template<class E, class P>
		concept can_static_query = 
			requires
		{
			std::decay_t<P>::template static_query_v<std::decay_t<E>>;
		} && std::decay_t<P>::template static_query_v<std::decay_t<E>>;

		template<class E, class P>
		concept can_member_call = 
			requires(E&& e, P&& p)
		{
			std::forward<E>(e).query(std::forward<P>(p));
		};

		struct cpo
		{
			private:
				enum class state { none, static_query, member_call, non_member_call };

				template<class E, class P>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (applicable<E, P>)
					{
						if constexpr (can_static_query<E, P>)
						{
							return {state::static_query, true};
						}
						else if constexpr (can_member_call<E, P>)
						{
							return {state::member_call, noexcept((std::declval<E>()).query(std::declval<P>()))};
						}
						else if constexpr (has_adl<E, P>)
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

				template<class E, class P>
				static constexpr auto choice = choose<E, P>();
			public:
				template<class E, class P>
					requires(choice<E, P>.strategy != state::none)
				constexpr decltype(auto) operator()(E&& e, P&& p) const noexcept(choice<E, P>.no_throw)
				{
					if constexpr (choice<E, P>.strategy == state::static_query)
					{
						return std::decay_t<P>::template static_query_v<std::decay_t<E>>;
					}
					else if constexpr (choice<E, P>.strategy == state::member_call)
					{
						return std::forward<E>(e).query(std::forward<P>(p));
					}
					else if constexpr (choice<E, P>.strategy == state::non_member_call)
					{
						return query(std::forward<E>(e), std::forward<P>(p));
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
		inline constexpr require_concept_cpo::cpo require_concept {};
		inline constexpr require_cpo::cpo require {};
		inline constexpr prefer_cpo::cpo prefer {};
		inline constexpr query_cpo::cpo query {};
	}

	using namespace cpos;

	template<class T, class P>
	struct can_require_concept : std::false_type {};

	template<class T, class P>
		requires(requires { cmoon::require_concept(std::declval<T>(), std::declval<P>()); })
	struct can_require_concept<T, P> : std::true_type {};

	template<class T, class... Ps>
	struct can_require : std::false_type {};

	template<class T, class... Ps>
		requires(requires { cmoon::require(std::declval<const T>(), std::declval<Ps>()...); })
	struct can_require<T, Ps...> : std::true_type {};

	template<class T, class... Ps>
	struct can_prefer : std::false_type {};

	template<class T, class... Ps>
		requires(requires { cmoon::prefer(std::declval<const T>(), std::declval<Ps>()...); })
	struct can_prefer<T, Ps...> : std::true_type {};

	template<class T, class P>
	struct can_query : std::false_type {};

	template<class T, class P>
		requires(requires { cmoon::query(std::declval<const T>(), std::declval<P>()); })
	struct can_query<T, P> : std::true_type {};

	template<class T, class P>
	inline constexpr bool can_require_concept_v = can_require_concept<T, P>::value;

	template<class T, class... Ps>
	inline constexpr bool can_require_v = can_require<T, Ps...>::value;

	template<class T, class... Ps>
	inline constexpr bool can_prefer_v = can_prefer<T, Ps...>::value;

	template<class T, class P>
	inline constexpr bool can_query_v = can_query<T, P>::value;
}