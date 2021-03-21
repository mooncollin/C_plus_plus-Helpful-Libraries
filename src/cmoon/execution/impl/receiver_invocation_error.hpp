#pragma once

#include <stdexcept>

namespace cmoon
{
	namespace execution
	{
		struct receiver_invocation_error : public std::runtime_error, public std::nested_exception
		{
			receiver_invocation_error() noexcept
				: std::runtime_error{"invocation-error"}, std::nested_exception{} {}
		};
	}
}