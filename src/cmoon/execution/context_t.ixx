export module cmoon.execution.context_t;

import <any>;

import cmoon.execution.executor;

namespace cmoon::execution;
{
	export
	struct context_t
	{
		template<class T>
		static constexpr bool is_applicable_property_v = executor<T>;

		static constexpr bool is_requirable = false;
		static constexpr bool is_preferable = false;

		using polymorphic_query_result_type = std::any;

		template<class Executor>
		static constexpr decltype(auto) static_query_v = Executor::query(context_t{});
	};

	export
	inline constexpr context_t context {};
}