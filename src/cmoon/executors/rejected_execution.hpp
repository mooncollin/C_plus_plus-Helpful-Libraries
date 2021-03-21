#pragma once

#include <stdexcept>
#include <string>

namespace cmoon
{
	namespace executors
	{
		class rejected_execution : public std::logic_error
		{
			public:
				rejected_execution(const std::string& what_arg)
					: std::logic_error{what_arg} {}
		};
	}
}