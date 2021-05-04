export module cmoon.execution.prefer_only;

import <utility>;

import cmoon.property;

namespace cmoon::execution
{
	template<class Property>
	struct polymorphic_query_result_type_extension {};

	template<class Property>
		requires (requires { typename Property::polymorphic_query_result_type; })
	struct polymorphic_query_result_type_extension<Property>
	{
		using polymorphic_query_result_type = typename Property::polymorphic_query_result_type;
	};

	export
	template<class Property>
	struct prefer_only : polymorphic_query_result_type_extension<Property>
	{
		Property property;

		static constexpr bool is_requirable = false;
		static constexpr bool is_preferable = Property::is_preferable;

		template<class Executor>
		static constexpr auto static_query_v = Property::template static_query_v<Executor>;

		constexpr prefer_only(const Property& p) noexcept
			: property{p} {}

		constexpr auto value() const noexcept(noexcept(std::declval<const Property>().value()))
		{
			return property.value();
		}

		template<class Executor, class Property2>
			requires(requires(Executor ex, prefer_only p) { cmoon::prefer(std::move(ex), p.property); })
		friend constexpr auto prefer(Executor ex, const prefer_only& p) noexcept(noexcept(cmoon::prefer(std::move(ex), std::declval<const Property>())))
		{
			return cmoon::prefer(std::move(ex), p.property);
		}

		template<class Executor, class Property2>
			requires(requires(Executor ex, prefer_only p) { cmoon::query(ex, p.property); })
		friend constexpr auto query(Executor ex, const prefer_only& p) noexcept(noexcept(cmoon::query(ex, std::declval<const Property>())))
		{
			return cmoon::query(ex, p.property);
		}
	};
}