#pragma once

#include <type_traits>

#include "cmoon/type_traits.hpp"
#include "cmoon/execution/impl/executor_shape.hpp"
#include "cmoon/execution/impl/bulk_guarantee_t.hpp"

namespace cmoon
{
	namespace execution
	{
		template<class Executor>
		struct executor_index
		{
			private:
				template<class T>
				using helper = typename T::index_type;
			public:
				using type = cmoon::detected_or_t<
				executor_shape_t<Executor>, helper, decltype(cmoon::require(std::declval<const Executor&>(), execution::bulk_guarentee_t))
				>;

				static_assert(std::is_integral_v<type>, "index type must be an integral type");
		};

		template<class Executor>
		using executor_index_t = typename executor_index<Executor>::type;
	}
}