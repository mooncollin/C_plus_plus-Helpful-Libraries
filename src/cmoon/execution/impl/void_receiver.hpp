#pragma once

#include <stdexcept>

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			struct void_receiver
			{
				void set_value() noexcept {}
				void set_error(std::exception_ptr) noexcept {}
				void set_done() noexcept {}
			};
		}
	}
}