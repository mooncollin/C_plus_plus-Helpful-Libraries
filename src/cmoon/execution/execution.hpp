#pragma once

#include <stdexcept>
#include <exception>
#include <concepts>
#include <type_traits>
#include <memory>
#include <optional>
#include <any>
#include <functional>

#include "cmoon/utility.hpp"
#include "cmoon/property.hpp"
#include "cmoon/type_traits.hpp"

#include "cmoon/execution/impl/receiver_invocation_error.hpp"
#include "cmoon/execution/impl/invocable_archetype.hpp"
#include "cmoon/execution/impl/set_value.hpp"
#include "cmoon/execution/impl/set_done.hpp"
#include "cmoon/execution/impl/set_error.hpp"
#include "cmoon/execution/impl/execute.hpp"
#include "cmoon/execution/impl/connect.hpp"
#include "cmoon/execution/impl/start.hpp"
#include "cmoon/execution/impl/submit.hpp"
#include "cmoon/execution/impl/schedule.hpp"
#include "cmoon/execution/impl/bulk_execute.hpp"
#include "cmoon/execution/impl/receiver.hpp"
#include "cmoon/execution/impl/operation_state.hpp"
#include "cmoon/execution/impl/sender.hpp"
#include "cmoon/execution/impl/executor.hpp"
#include "cmoon/execution/impl/context_t.hpp"
#include "cmoon/execution/impl/blocking_t.hpp"
#include "cmoon/execution/impl/relationship_t.hpp"
#include "cmoon/execution/impl/outstanding_work_t.hpp"
#include "cmoon/execution/impl/bulk_guarantee_t.hpp"
#include "cmoon/execution/impl/mapping_t.hpp"
#include "cmoon/execution/impl/allocator_t.hpp"
//#include "cmoon/execution/impl/twoway_t.hpp"
#include "cmoon/execution/impl/executor_shape.hpp"
#include "cmoon/execution/impl/executor_index.hpp"
//
#include "cmoon/execution/impl/any_executor.hpp"
#include "cmoon/execution/impl/bad_executor.hpp"
#include "cmoon/execution/impl/prefer_only.hpp"
#include "cmoon/execution/impl/inline_executor.hpp"
//
//#include "cmoon/execution/impl/executor_future.hpp"
//#include "cmoon/execution/impl/is_twoway_executor.hpp"
//#include "cmoon/execution/impl/is_then_executor.hpp"
//#include "cmoon/execution/impl/is_bulk_twoway_executor.hpp"
//#include "cmoon/execution/impl/is_bulk_then_executor.hpp"

#include "cmoon/execution/impl/find_convertible_property.hpp"

namespace cmoon
{
	namespace execution
	{
		template<class Property, class... SupportableProperties>
			requires(requires { details::find_convertible_property<Property, SupportableProperties...>::is_preferable; } && 
						details::find_convertible_property<Property, SupportableProperties...>::is_preferable)
		any_executor<SupportableProperties...> prefer(const any_executor<SupportableProperties...>& e, Property p)
		{
			// TODO
		}
	}
}

namespace std
{
	template<class... SupportableProperties>
	void swap(cmoon::execution::any_executor<SupportableProperties...>& a, cmoon::execution::any_executor<SupportableProperties...>& b) noexcept
	{
		a.swap(b);
	}
}