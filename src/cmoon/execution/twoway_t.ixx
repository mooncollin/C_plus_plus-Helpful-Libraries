export module cmoon.execution.twoway_t;

import cmoon.execution.is_twoway_executor;

namespace cmoon::execution
{
	export
	struct twoway_t
	{
		private:
			enum class state {};
		public:
			template<class Executor>
				requires(is_twoway_executor_v<Executor>)
			friend decltype(auto) require(Executor ex, twoway_t);
	};

	export
	inline constexpr twoway_t twoway {};
}