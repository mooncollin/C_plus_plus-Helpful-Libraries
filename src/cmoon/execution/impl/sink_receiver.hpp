#pragma once

#include <exception>

namespace cmoon
{
	namespace execution
	{
		class sink_receiver
		{
			public:
				template<class... Args>
				void set_value(Args&&...) {}

				template<class E>
				[[noreturn]] void set_error(E&&) noexcept
				{
					std::terminate();
				}

				void set_done() noexcept {}
		};
	}
}