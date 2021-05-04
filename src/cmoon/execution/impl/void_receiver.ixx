export module cmoon.execution.void_receiver;

import <stdexcept>;

namespace cmoon::execution
{
	export
	struct void_receiver
	{
		void set_value() noexcept {}
		void set_error(std::exception_ptr) noexcept {}
		void set_done() noexcept {}
	};
}