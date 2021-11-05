export module cmoon.execution.prefer_only;

import <utility>;

import cmoon.property;

namespace cmoon::execution
{
	template<class InnerProperty>
	struct base_prefer_only
	{
		InnerProperty property;

		static constexpr bool is_requirable {false};
		static constexpr bool is_preferable {InnerProperty::is_preferable};

		template<class Executor>
		static constexpr auto static_query_v {InnerProperty::template static_query_v<Executor>};

		constexpr base_prefer_only(const InnerProperty& p)
			: property{p} {}

		constexpr auto value() const noexcept(noexcept(std::declval<const InnerProperty>().value()))
		{
			return property.value();
		}

		template<class Executor>
		friend constexpr auto prefer(Executor ex, const base_prefer_only& p) noexcept(noexcept(cmoon::prefer(std::move(ex), std::declval<const InnerProperty>())))
		{
			return cmoon::prefer(std::move(ex), p.property);
		}

		template<class Executor>
		friend constexpr auto query(Executor ex, const base_prefer_only& p) noexcept(noexcept(cmoon::query(ex, std::declval<const InnerProperty>())))
		{
			return cmoon::query(ex, p.property);
		}
	};

	export
	template<class InnerProperty>
	struct prefer_only : public base_prefer_only<InnerProperty> {};

	export
	template<class InnerProperty>
		requires(requires { typename InnerProperty::polymorphic_query_result_type; })
	struct prefer_only<InnerProperty> : public base_prefer_only<InnerProperty>
	{
		using polymorphic_query_result_type = typename InnerProperty::polymorphic_query_result_type;
	};
}