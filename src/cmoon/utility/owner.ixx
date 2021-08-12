export module cmoon.utility.owner;

import <type_traits>;

namespace cmoon
{
	export
	template<class T>
		requires(std::is_pointer_v<T>)
	using owner = T;
}