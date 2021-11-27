export module cmoon.execution.forward_progress_guarantee;

namespace cmoon::execution
{
	export
	enum class forward_progress_guarantee
	{
		concurrent,
		parallel,
		weakly_parallel
	};
}