#pragma once

#include <type_traits>

namespace collin
{
	template<class T, class P>
	struct is_applicable_property;

	template<class T, class Property>
	constexpr bool is_applicable_property_v = is_applicable_property<T, Property>::value;

	inline namespace unspecified
	{
		template<class E, class P, typename T = std::decay_t<E>, typename Prop = std::decay_t<P>>
		concept static_query_base =
			requires
		{
			Prop::template static_query_v<T>;
			Prop::value();
		} && Prop::template static_query_v<T> == Prop::value();

		template<class E, class P, typename T = std::decay_t<E>, typename Prop = std::decay_t<P>>
		concept require_concept_base = 
		requires
		{
			is_applicable_property_v<T, Prop>;
		} && is_applicable_property_v<T, Prop> &&
			requires
		{
			Prop::is_requirable_concept;
		} && Prop::is_requireable_concept;
		
		struct require_concept_impl
		{
			template<class E, class P>
				requires(require_concept_base<E, P> && static_query_base<E, P>)
			constexpr auto operator()(E e, P p) const
			{
				return e;
			}

			template<class E, class P>
				requires(require_concept_base<E, P> && requires(E e, P p) { e.require_concept(p); })
			constexpr auto operator()(E e, P p) const
			{
				return e.require_concept(p);
			}

			template<class E, class P>
				requires(require_concept_base<E, P> && requires(E e, P p) { require_concept(e, p); })
			constexpr auto operator()(E e, P p) const
			{
				return require_concept(e, p);
			}
		};

		constexpr auto require_concept = require_concept_impl{};

		template<class E, class P0, class... Pn, typename T = std::decay_t<E>, typename Prop0 = std::decay_t<P0>>
		concept require_base = 
			requires
		{
			is_applicable_property_v<T, Prop0>;
		}&& is_applicable_property_v<T, Prop0>&&
			requires
		{
			Prop0::is_requirable;
		} && Prop0::is_requirable;

		struct require_impl
		{
			template<class E, class P0, class... Pn>
				requires(require_base<E, P0, Pn...> && static_query_base<E, P0> && (sizeof...(Pn) == 0))
			constexpr auto operator()(E e, P0 p, Pn...) const
			{
				return e;
			}

			template<class E, class P0, class... Pn>
				requires(require_base<E, P0, Pn...> && (sizeof...(Pn) == 0) && requires(E e, P0 p) { e.require(p); })
			constexpr auto operator()(E e, P0 p, Pn...) const
			{
				return e.require(p);
			}

			template<class E, class P0, class... Pn>
				requires(require_base<E, P0, Pn...> && (sizeof...(Pn) == 0) && requires(E e, P0 p) { require(e, p); })
			constexpr auto operator()(E e, P0 p, Pn...) const
			{
				return require(e, p);
			}

			template<class E, class P0, class... Pn>
				requires((sizeof...(Pn) > 0) && requires(E e, P0 p, Pn... pn) { require(require(e, p), pn...); })
			constexpr auto operator()(E e, P0 p, Pn... pn) const
			{
				return require(require(e, p0), pn...);
			}
		};

		constexpr auto require = require_impl{};

		template<class E, class P, typename T = std::decay_t<E>, typename Prop = std::decay<P>>
		concept prefer_base = 
			requires
		{
			is_applicable_property_v<T, Prop>;
		}&& is_applicable_property_v<T, Prop>&&
			requires
		{
			Prop::is_preferable;
		} && Prop::is_preferable;

		struct prefer_impl
		{
			template<class E, class P0, class... Pn>
				requires(prefer_base<E, P0> && (sizeof...(Pn) == 0) && static_query_base<E, P0>)
			constexpr auto operator()(E e, P0 p, Pn...) const
			{
				return e;
			}

			template<class E, class P0, class... Pn>
				requires(prefer_base<E, P0> && (sizeof...(Pn) == 0) && requires(E e, P0 p) { e.prefer(p); })
			constexpr auto operator()(E e, P0 p, Pn...) const
			{
				return e.prefer(p);
			}

			template<class E, class P0, class... Pn>
				requires(prefer_base<E, P0> && (sizeof...(Pn) == 0) && requires(E e, P0 p) { prefer(e, p); })
			constexpr auto operator()(E e, P0 p, Pn...) const
			{
				return prefer(e, p);
			}

			template<class E, class P0, class... Pn>
				requires(prefer_base<E, P0> && (sizeof...(Pn) > 0) && requires(E e, P0 p, Pn... pn) { prefer(prefer(e, p), pn...); })
			constexpr auto operator()(E e, P0 p, Pn...) const
			{
				return prefer(prefer(e, p), pn...);
			}
		};

		constexpr auto prefer = prefer_impl{};

		template<class E, class P, typename T = std::decay_t<E>, typename Prop = std::decay_t<P>>
		concept query_base = 
			requires
		{
			is_applicable_property_v<T, Prop>;
		} && is_applicable_property_v<T, Prop>;

		struct query_impl
		{
			template<class E, class P, typename T = std::decay_t<E>, typename Prop = std::decay_t<P>>
				requires(query_base<E, P> && requires { Prop::template static_query_v<T>; })
			constexpr auto operator()(E e, P p) const
			{
				return Prop::template static_query_v<T>;
			}

			template<class E, class P>
				requires(query_base<E, P> && requires(E e, P p) { e.query(p); })
			constexpr auto operator()(E e, P p) const
			{
				return e.query(p);
			}

			template<class E, class P>
				requires(query_base<E, P> && requires(E e, P p) { query(e, p); })
			constexpr auto operator()(E e, P p) const
			{
				return query(e, p);
			}
		};

		constexpr auto query = query_impl{};
	}

	template<class T, class Property>
	struct is_applicable_property : std::false_type {};

	template<class T, class Property>
		requires(requires { Property::template is_applicable_property_v<T>; } && Property::template is_applicable_property_v<T>)
	struct is_applicable_property<T, Property> : std::true_type {};

	template<class T, class Property>
	struct can_require_concept : std::false_type {};

	template<class T, class Property>
		requires(requires { collin::require_concept(std::declval<const T>(), std::declval<Property>()); })
	struct can_require_concept<T, Property> : std::true_type {};

	template<class T, class... Properties>
	struct can_require : std::false_type {};

	template<class T, class... Properties>
		requires(requires { collin::require(std::declval<const T>(), std::declval<Properties>()...); })
	struct can_require<T, Properties...> : std::true_type {};

	template<class T, class... Properties>
	struct can_prefer : std::false_type {};

	template<class T, class... Properties>
		requires(requires { collin::prefer(std::declval<const T>(), std::declval<Properties>()...); })
	struct can_prefer<T, Properties...> : std::true_type {};

	template<class T, class Property>
	struct can_query : std::false_type {};

	template<class T, class Property>
		requires(requires { Property::template is_applicable_property_v<T>; } && Property::template is_applicable_property_v<T>)
	struct can_query<T, Property> : std::true_type {};

	template<class T, class Property>
	constexpr bool can_require_concept_v = can_require_concept<T, Property>::value;

	template<class T, class... Properties>
	constexpr bool can_require_v = can_require<T, Properties...>::value;

	template<class T, class... Properties>
	constexpr bool can_prefer_v = can_prefer<T, Properties...>::value;

	template<class T, class Property>
	constexpr bool can_query_v = can_query<T, Property>::value;
}