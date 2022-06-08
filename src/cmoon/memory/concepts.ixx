export module cmoon.memory.concepts;

import <memory>;

namespace cmoon
{
	export
	template<class Alloc>
	concept allocator =
		requires
	{
		typename std::allocator_traits<Alloc>;
	};

	export
	template<class Alloc, class T>
	concept allocates_type = allocator<Alloc> &&
							 std::same_as<typename std::allocator_traits<Alloc>::value_type, T>;
}