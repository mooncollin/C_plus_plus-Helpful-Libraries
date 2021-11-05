export module cmoon.utility.as_unsigned;

import <type_traits>;

namespace cmoon
{
	export
	template<class T>
		requires(requires {
			typename std::make_unsigned<T>;
		})
	constexpr std::make_unsigned_t<T> as_unsigned(T t) noexcept(std::is_nothrow_constructible_v<std::make_unsigned_t<T>, T>)
	{
		return static_cast<std::make_unsigned_t<T>>(t);
	}
}