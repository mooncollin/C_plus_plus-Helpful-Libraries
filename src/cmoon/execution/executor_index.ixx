export module cmoon.execution.executor_index;

import <concepts>;
import <utility>;

import cmoon.type_traits;
import cmoon.property;

import cmoon.execution.bulk_guarantee_t;
import cmoon.execution.executor_shape;

namespace cmoon::execution
{
	export
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

			static_assert(std::integral<type>, "index type must be an integral type");
	};

	export
	template<class Executor>
	using executor_index_t = typename executor_index<Executor>::type;
}