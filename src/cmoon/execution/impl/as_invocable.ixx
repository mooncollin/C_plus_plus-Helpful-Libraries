export module cmoon.execution.impl.as_invocable;

import <memory>;
import <utility>;
import <exception>;

import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.set_done;

namespace cmoon::execution
{
	export
	template<class R, class>
	struct as_invocable
	{
		R* r_;

		explicit as_invocable(R& r) noexcept
			: r_{std::addressof(r)} {}

		as_invocable(as_invocable&& other) noexcept
			: r_{std::exchange(other.r_, nullptr)} {}

		~as_invocable()
		{
			if (r_)
			{
				execution::set_done(std::move(*r_));
			}
		}

		void operator()() & noexcept try
		{
			execution::set_value(std::move(*r_));
			r_ = nullptr;
		}
		catch (...)
		{
			execution::set_error(std::move(*r_), std::current_exception());
			r_ = nullptr;
		}
	};
}