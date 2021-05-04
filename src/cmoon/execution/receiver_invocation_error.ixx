export module cmoon.execution.receiver_invocation_error;

import <stdexcept>;

namespace cmoon::execution
{
	export
	struct receiver_invocation_error : public std::runtime_error, public std::nested_exception
	{
		receiver_invocation_error() noexcept
			: std::runtime_error{"invocation-error"}, std::nested_exception{} {}
	};
}