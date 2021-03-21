#pragma once

#include <future>
#include <type_traits>

#include "cmoon/execution/impl/is_future.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class F, class Future, class Val>
				requires(details::is_future_v<Future> &&
						 std::same_as<Future, std::future<void>>)
			struct normal_type_impl : std::type_identity<std::invoke_result_t<std::decay_t<F>>> {};

			template<class F, class Future, class Val>
				requires(details::is_future_v<Future> &&
						 !std::same_as<Future, std::future<void>>)
			struct normal_type_impl<F, Future, Val> : std::type_identity<std::invoke_result_t<std::decay_t<F>, Val>> {};

			template<class F, class Future, class Val>
			using normal_type_impl_t = typename normal_type_impl<F, Future, Val>::type;
		}
	}
}