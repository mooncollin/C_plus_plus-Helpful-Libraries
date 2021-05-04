export module cmoon.execution.executor_shape;

import <utility>;
import <concepts>;

import cmoon.type_traits;

import cmoon.execution.bulk_guarantee_t;

namespace cmoon::execution
{
	export
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

			static_assert(std::integral<type>, "shape type must be an integral type");
	};

	export
	template<class Executor>
	using executor_shape_t = typename executor_shape<Executor>::type;
}