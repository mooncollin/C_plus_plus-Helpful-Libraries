#pragma once

#include <stdexcept>
#include <string>

namespace cmoon
{
	namespace executors
	{
		class cancelled_execution : public std::runtime_error
		{
			public:
				cancelled_execution(const std::string& what_arg)
					: std::runtime_error{what_arg} {}
		};
	}
}