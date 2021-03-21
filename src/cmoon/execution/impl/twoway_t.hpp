#pragma once

#include "cmoon/execution/impl/is_twoway_executor.hpp"

namespace cmoon
{
	namespace execution
	{
		struct twoway_t
		{
			private:
				enum class state {  };
			public:
				template<class Executor>
					requires(is_twoway_executor_v<Executor>)
				friend decltype(auto) require(Executor ex, twoway_t);
		};

		inline constexpr twoway_t twoway {};
	}
}