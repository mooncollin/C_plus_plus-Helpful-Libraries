#pragma once
import cmoon.core;

#include <utility>
#include <stdexcept>

//#include "cmoon/memory.hpp"
#include "cmoon/execution/impl/receiver.hpp"
#include "cmoon/execution/impl/set_value.hpp"
#include "cmoon/execution/impl/set_error.hpp"
#include "cmoon/execution/impl/set_done.hpp"


namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<receiver<> R, class>
			struct as_invocable
			{
				cmoon::observer_ptr<R> r_;

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

				void operator()() noexcept try
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
	}
}