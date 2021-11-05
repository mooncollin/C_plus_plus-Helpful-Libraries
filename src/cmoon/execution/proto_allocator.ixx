export module cmoon.execution.proto_allocator;

import <concepts>;
import <memory>;

namespace cmoon::execution
{
	export
	template<class A>
	concept proto_allocator = std::copy_constructible<A> &&
		requires
	{
		typename std::allocator_traits<A>::template rebind_alloc<int>;
	};
}