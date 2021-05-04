export module cmoon.execution.normal_type_impl;

import <future>;
import <type_traits>;
import <concepts>;

import cmoon.type_traits;

namespace cmoon::execution
{
	export
	template<class F, class Future, class Val>
		requires(cmoon::is_future_v<Future> &&
				 std::same_as<Future, std::future<void>>)
	struct normal_type_impl : std::type_identity<std::invoke_result_t<std::decay_t<F>>> {};

	export
	template<class F, class Future, class Val>
		requires(cmoon::is_future_v<Future> &&
				 !std::same_as<Future, std::future<void>>)
	struct normal_type_impl<F, Future, Val> : std::type_identity<std::invoke_result_t<std::decay_t<F>, Val>> {};

	export
	template<class F, class Future, class Val>
	using normal_type_impl_t = typename normal_type_impl<F, Future, Val>::type;
}