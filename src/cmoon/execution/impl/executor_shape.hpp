#pragma once

#include <type_traits>

#include "cmoon/type_traits.hpp"
#include "cmoon/execution/impl/bulk_guarantee_t.hpp"

namespace cmoon
{
	namespace execution
	{
		template<class Executor>
		struct executor_shape
		{
			private:
				template<class T>
				using helper = typename T::shape_type;
			public:
				using type = cmoon::detected_or_t<
				std::size_t, helper, decltype(cmoon::require(std::declval<const Executor&>(), execution::bulk_guarentee_t))
				>;

				static_assert(std::is_integral_v<type>, "shape type must be an integral type");
		};

		template<class Executor>
		using executor_shape_t = typename executor_shape<Executor>::type;
	}
}