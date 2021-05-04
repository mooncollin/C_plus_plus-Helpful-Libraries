export module cmoon.execution.as_receiver;

import <concepts>;

namespace cmoon::execution
{
	export
	template<std::invocable<> F, class>
	struct as_receiver
	{
		F f_;

		void set_value() noexcept(std::is_nothrow_invocable_v<F&>)
		{
			std::invoke(f_);
		}

		template<class E>
		[[noreturn]] void set_error(E&&) noexcept
		{
			std::terminate();
		}

		void set_done() noexcept {}
	};
}