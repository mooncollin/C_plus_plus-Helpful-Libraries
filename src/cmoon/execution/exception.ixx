export module cmoon.execution.exception;

namespace cmoon::execution
{
	export
	struct execption_arg_t
	{
		explicit execption_arg_t() = default;
	};

	export
	inline constexpr execption_arg_t execption_arg {};
}